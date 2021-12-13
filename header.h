#ifndef HEADER_H
// If macro 'HEADER_H' is not defined, define it now
#define HEADER_H

// Includes
#include <stdio.h>
#include <stddef.h>

// Defines

// Data Structs

struct Product {
	size_t id;
	size_t price;
	char name[32];
	size_t stock;
};

struct Receipt {
	size_t id;
	size_t totalPrice;
	char name[32];
};

/*
struct Footer {
	char msg[150];
};
*/

// Function Prototypes

void show_catalog(char* catalog, struct Product product);
void modify_catalog(char* catalog, struct Product product);
void update_catalog(FILE* cfp, struct Product product);
void fill_receipt(FILE* rfp, char* receipt, struct Receipt rc);
// void end_receipt(FILE* rfp, char* receipt, struct Footer footer, size_t fsize);
void buy_product(char* catalog, char* receipt, struct Product product, struct Receipt rc);
void print_receipt(char* receipt, struct Receipt rc);

#endif
