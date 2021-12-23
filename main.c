// Standard lib includes
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Header file includes
#include "header.h"

int main(int argc, char* argv[argc + 1]) {
	if (argc != 5) {
		fprintf(stderr, "Usage: %s <catalog_file> <economy_file> <receipt_file> <history_file>\n", argv[0]);
		return -1;
	}

	struct Product product = {0};
	struct Receipt rc = {0};

	printf("Welcome to Store Cashier Program!\nMade with ~love~ by Richie Seputro\n");

	int choice;
	int menu = 1;
	while (menu) {
		printf("(1) Show Catalog\n(2) Modify Catalog\n(3) Finalize Catalog\n(4) Buy Product\n(5) Print Receipt\n(6) Show History\n(7) Show Economy\n(0) Exit Program\nEnter your choice: ");
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
				finalize_catalog(argv[1], argv[2], product);
				break;
			case 4:
				buy_product(argv[1], argv[3], product, rc);
				break;
			case 5:
				print_receipt(argv[3], argv[4], rc);
				break;
			case 6:
				show_history(argv[4], rc);
				break;
			case 7:
				show_economy(argv[2], argv[4], rc);
		}
	}

	return 0;
}
