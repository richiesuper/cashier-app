// Includes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Function definitions

void show_catalog(char* catalog, struct Product product) {
	FILE* cfp = fopen(catalog, "rb");
	if (cfp == NULL) {
		perror(catalog);
		exit(-2);
	}

	printf("\n\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Stock", "Price");
	printf("---------------------------------------------------------\n");

	size_t ret = fread(&product, sizeof(struct Product), 1, cfp);
	while (ret) {
		printf("%-7zu%-33s%7zu%10zu\n", product.id, product.name, product.stock, product.price);
		ret = fread(&product, sizeof(struct Product), 1, cfp);
	}

	fclose(cfp);
	printf("\n");
}

void modify_catalog(char* catalog, struct Product product) {
	FILE* cfp = fopen(catalog, "rb+");
	if (cfp == NULL) {
		perror(catalog);
		exit(-3);
	}

	int id;

	/* Product Catalog Specifications */
	printf("\n\nCatalog product ID's range is from 0 to 999999\nTo end input, enter any negative number when prompted for ID\n");
	printf("Product names must be between 0 and 32 characters long, inclusive\n");
	printf("Product stocks must be between 0 and 999999, inclusive\n");
	printf("Product prices must be between 0 and 999999999, inclusive\n\n");

	while (1) {
		printf("Enter product ID:\n> ");
		scanf("%d", &id);

		if (id < 0) {
			break;
		}

		product.id = id;

		printf("Enter product name:\n> ");
		scanf(" %[^\n]", product.name);
		printf("Enter product stock and price:\n> ");
		scanf("%zu %zu", &product.stock, &product.price);

		fseek(cfp, product.id * sizeof(struct Product), SEEK_SET);

		fwrite(&product, sizeof(struct Product), 1, cfp);
	}

	fclose(cfp);
	printf("\n\n");
}

void update_catalog(FILE* cfp, struct Product product) {
	fseek(cfp, product.id * sizeof(struct Product), SEEK_SET);
	fwrite(&product, sizeof(struct Product), 1, cfp);
}

void finalize_catalog(char* catalog, char* economy, struct Product product) {
	FILE* cfp = fopen(catalog, "rb");
	if (cfp == NULL) {
		perror(catalog);
		exit(-4);
	}

	FILE* efp = fopen(economy, "wb");
	if (efp == NULL) {
		perror(economy);
		exit(-5);
	}

	size_t capital = 0;
	size_t ret = fread(&product, sizeof(struct Product), 1, cfp);
	while (ret) {
		capital += product.price * product.stock;
		ret = fread(&product, sizeof(struct Product), 1, cfp);
	}

	fwrite(&capital, sizeof(size_t), 1, efp);

	printf("\n\nOK, catalog has been finalized!\n\n");

	fclose(cfp);
	fclose(efp);
}

void show_economy(char* economy, char* history, struct Receipt rc) {
	FILE* efp = fopen(economy, "rb");
	if (efp == NULL) {
		perror(economy);
		exit(6);
	}

	FILE* hfp = fopen(history, "rb");
	if (hfp == NULL) {
		perror(history);
		exit(7);
	}

	size_t capital;
	size_t revenue = 0;
	long long balance;
	size_t ret = fread(&capital, sizeof(size_t), 1, efp);
	if (ret) {
		ret = fread(&rc, sizeof(struct Receipt), 1, hfp);
		while (ret) {
			revenue += rc.totalPrice;
			ret = fread(&rc, sizeof(struct Receipt), 1, hfp);
		}

		balance = -capital + revenue;
		printf("\n\nInitial Capital: %zu\nRevenue: %zu\nBalance: %lld\n\n", capital, revenue, balance);
	} else {
		printf("\n\nYou have NOT finalized the catalog! Aborting!\n\n");
	}

	fclose(efp);
	fclose(hfp);
}

void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc) {
	fwrite(&rc, sizeof(struct Receipt), 1, rfp);
	freopen(receipt, "ab", rfp);
}

void buy_product(char* catalog, char* receipt, struct Product product, struct Receipt rc) {
	FILE* cfp = fopen(catalog, "rb+");
	if (cfp == NULL) {
		perror(catalog);
		exit(-8);
	}

	FILE* rfp = fopen(receipt, "ab");
	if (rfp == NULL) {
		perror(receipt);
		exit(-9);
	}

	int id;
	size_t items;
	size_t payout = 0;

	printf("\nCatalog product ID's range is from 0 to 999999\nTo end input, enter any negative number when prompted for ID\n");

	while (1) {
		printf("Enter product ID:\n> ");
		scanf("%d", &id);

		if (id < 0) {
			break;
		}

		fseek(cfp, id * sizeof(struct Product), SEEK_SET);
		fread(&product, sizeof(struct Product), 1, cfp);

		printf("\n\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Stock", "Price");
		printf("%-7zu%-33s%7zu%10zu\n", product.id, product.name, product.stock, product.price);

		if (product.stock <= 0) {
			printf("Sorry, %s is currently out of stock right now!\n\n", product.name);
			continue;
		}

		do {
			printf("Enter how many %s you want to buy:\n> ", product.name);
			scanf("%zu", &items);

			if (items > product.stock) {
				printf("Error, you can not buy more than the available stock!\n\n");
			}
		} while (items > product.stock);

		if (items != 0) {
			printf("OK, bought %zu %s which costs %zu per item.\n\n", items, product.name, product.price);

			product.stock -= items;

			update_catalog(cfp, product);

			payout += product.price * items;

			rc.id = id;
			strncpy(rc.name, product.name, 32);
			rc.totalPrice = payout;
			rc.num = items;

			payout = 0;

			fill_receipt(rfp, receipt, rc);
		} else {
			printf("OK, aborting purchase!\n\n");
		}
	}

	fclose(cfp);
	fclose(rfp);
	printf("\n");
}

void print_receipt(char* receipt, char* history, struct Receipt rc) {
	FILE* rfp = fopen(receipt, "rb");
	if (rfp == NULL) {
		perror(receipt);
		exit(-10);
	}

	printf("\n\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Items", "Price");
	printf("---------------------------------------------------------\n");

	size_t total = 0;
	size_t ret = fread(&rc, sizeof(struct Receipt), 1, rfp);
	while (ret) {
		printf("%-7zu%-33s%7zu%10zu\n", rc.id, rc.name, rc.num, rc.totalPrice);
		total += rc.totalPrice;
		ret = fread(&rc, sizeof(struct Receipt), 1, rfp);
	}

	fclose(rfp);

	printf("---------------------------------------------------------\nTOTAL: %10zu\n\n", total);

	write_history(receipt, history, rc);
	destroy_receipt(receipt);
}

void write_history(char* receipt, char* history, struct Receipt rc) {
	FILE* rfp = fopen(receipt, "rb");
	if (rfp == NULL) {
		perror(receipt);
		exit(-11);
	}

	FILE* hfp = fopen(history, "ab");
	if (hfp == NULL) {
		perror(history);
		exit(-12);
	}

	size_t ret = fread(&rc, sizeof(struct Receipt), 1, rfp);

	while (ret) {
		fwrite(&rc, sizeof(struct Receipt), 1, hfp);
		ret = fread(&rc, sizeof(struct Receipt), 1, rfp);
	}

	fclose(hfp);
	fclose(rfp);
}

void destroy_receipt(char* receipt) {
	FILE* rfp = fopen(receipt, "wb");
	if (rfp == NULL) {
		perror(receipt);
		exit(-13);
	}

	fclose(rfp);
}

void show_history(char* history, struct Receipt rc) {
	FILE* hfp = fopen(history, "rb");
	if (hfp == NULL) {
		perror(history);
		exit(-14);
	}

	printf("\n\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Items", "Price");
	printf("---------------------------------------------------------\n");

	size_t total = 0;
	size_t ret = fread(&rc, sizeof(struct Receipt), 1, hfp);
	while (ret) {
		printf("%-7zu%-33s%7zu%10zu\n", rc.id, rc.name, rc.num, rc.totalPrice);
		total += rc.totalPrice;
		ret = fread(&rc, sizeof(struct Receipt), 1, hfp);
	}

	fclose(hfp);

	printf("---------------------------------------------------------\nREVENUE: %10zu\n\n", total);
}
