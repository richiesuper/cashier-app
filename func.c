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

	printf("\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Stock", "Price");

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
	printf("\nCatalog product ID's range is from 0 to 999999\nTo end input, enter any negative number when prompted for ID\n");
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
	printf("\n");
}

void update_catalog(FILE* cfp, struct Product product) {
	fseek(cfp, product.id * sizeof(struct Product), SEEK_SET);
	fwrite(&product, sizeof(struct Product), 1, cfp);
}

void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc) {
	fwrite(&rc, sizeof(struct Receipt), 1, rfp);
	freopen(receipt, "ab", rfp);
}

void end_receipt(FILE* rfp, char* receipt, struct Footer footer, size_t fsize) {
	fwrite(&footer, sizeof(char), fsize, rfp);
	freopen(receipt, "ab", rfp);
}

void buy_product(char* catalog, char* receipt, struct Product product, struct Receipt rc, struct Footer footer) {
	FILE* cfp = fopen(catalog, "rb+");
	if (cfp == NULL) {
		perror(catalog);
		exit(-4);
	}

	FILE* rfp = fopen(receipt, "ab");
	if (rfp == NULL) {
		perror(receipt);
		exit(-5);
	}

	int id;
	size_t items;
	size_t payout = 0;
	size_t totalPay = 0;

	printf("\nCatalog product ID's range is from 0 to 999999\nTo end input, enter any negative number when prompted for ID\n");

	while (1) {
		printf("Enter product ID:\n> ");
		scanf("%d", &id);

		if (id < 0) {
			break;
		}

		fseek(cfp, id * sizeof(struct Product), SEEK_SET);
		fread(&product, sizeof(struct Product), 1, cfp);

		printf("\n%-7s%-33s%7s%10s\n", "ID", "Product Name", "Stock", "Price");
		printf("%-7zu%-33s%7zu%10zu\n", product.id, product.name, product.stock, product.price);

		if (product.stock <= 0) {
			printf("Sorry, that product is currently out of stock right now!\n\n");
			continue;
		}

		do {
			printf("Enter how many products you want to buy:\n> ");
			scanf("%zu", &items);

			if (items > product.stock) {
				printf("Error, you can not buy more than the available stock!\n\n");
			}
		} while (items > product.stock);

		printf("OK, bought %zu %s which costs %zu per item.\n\n", items, product.name, product.price);

		product.stock -= items;

		update_catalog(cfp, product);

		payout += product.price * items;

		rc.id = id;
		strncpy(rc.name, product.name, 32);
		rc.totalPrice = payout;

		totalPay += payout;
		payout = 0;

		fill_receipt(rfp, receipt, rc);

	}

	snprintf(footer.msg, 150, "--------------------------------------------------\n                                                                  TOTAL: %10zu", totalPay);

	size_t fsize = strlen(footer.msg);
	end_receipt(rfp, receipt, footer, fsize);
	fclose(cfp);
	fclose(rfp);
	printf("\n");
}

void print_receipt(char* receipt, struct Receipt rc, struct Footer footer) {
	FILE* rfp = fopen(receipt, "rb");
	if (rfp == NULL) {
		perror(receipt);
		exit(-2);
	}

	size_t count = 0;

	size_t ret = fread(&rc, sizeof(struct Receipt), 1, rfp);
	while (ret) {
		ret = fread(&rc, sizeof(struct Product), 1, rfp);
		count++;
	}

	printf("\n%-7s%-33s%10s\n", "ID", "Product Name", "Price");

	count--;
	while (ret) {
		if (count) {
			printf("%-7zu%-33s%10zu\n", rc.id, rc.name, rc.totalPrice);
		} else {
			printf("%s\n", footer.msg);
		}

		ret = fread(&rc, sizeof(struct Product), 1, rfp);
	}

	fclose(rfp);
	printf("\n");
}
