#ifndef HEADER_H
// If macro 'HEADER_H' is not defined, define it now
#define HEADER_H

// Includes
#include <stdio.h>
#include <stddef.h>

// Data Structs

struct Product {
	size_t id;
	size_t price;
	size_t wholesalePrice;
	char name[32];
	size_t stock;
};

struct Receipt {
	size_t id;
	size_t totalPrice;
	char name[32];
	size_t num;
};

// Function Prototypes

void show_catalog(char* catalog, struct Product product);
void modify_catalog(char* catalog, struct Product product);
void update_catalog(FILE* cfp, struct Product product);
void finalize_catalog(char* catalog, char* economy, struct Product product);
void show_economy(char* economy, char* history, struct Receipt rc);
void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc);
void buy_product(char* catalog, char* receipt, struct Product product, struct Receipt rc);
void print_receipt(char* receipt, char* history, struct Receipt rc);
void write_history(char* receipt, char* history, struct Receipt rc);
void destroy_receipt(char* receipt);
void show_history(char* history, struct Receipt rc);

#endif
