// Standard lib includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Header file includes
#include "header.h"

int main(int argc, char* argv[argc + 1]) {
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <catalog_file> <receipt_file>\n", argv[0]);
		return -1;
	}

	struct Product product = {0};
	struct Receipt rc = {0};
	// struct Footer footer = {0};

	printf("Welcome to Store Cashier Program!\n");

	int choice;
	int menu = 1;
	while (menu) {
		printf("(1) Show Catalog\n(2) Modify Catalog\n(3) Buy Product\n(4) Print Receipt\n(0) Exit Program\nEnter your choice: ");
		scanf("%d", &choice);

		switch (choice) {
			default:
				printf("Invalid choice!\n\n");
				break;
			case 0:
				menu = 0;
				break;
			case 1:
				show_catalog(argv[1], product);
				break;
			case 2:
				modify_catalog(argv[1], product);
				break;
			case 3:
				buy_product(argv[1], argv[2], product, rc);
				break;
			case 4:
				print_receipt(argv[2], rc);
				break;
		}
	}

	return 0;
}
