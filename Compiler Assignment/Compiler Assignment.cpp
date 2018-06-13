// Compiler Assignment.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<stdlib.h>
#include<string.h>
#include<conio.h>


#define MEMORY_SIZE 1000


#pragma warning(disable:4996)

#include "global.h"
#include "string_utils.h"
#include "memory_utils.h"
#include "stack_utils.h"
#include "utils.h"
#include "hash.h"
#include "grammar.h"
#include "struct_operations.h"
#include "file_utils.h"
#include "intermediate_code_operations.h"



int _tmain(int argc, _TCHAR* argv[])
{
	
	initialize_optable();
	initialize_symboltable();
	initialize_instruction_table();
	initialize_label_table();
	initialize_constant_symboltable();
	char *source_filename = (char *)malloc(sizeof(char) * 10);
	strcpy(source_filename, "factorial.txt");
	//generate_intermediate_code(source_filename);
	//display_intermediate_table();
	//execute_intermediate_code();


	//generate_intermediate_code_file(source_filename,"test.o");
	//load_intermediate_code_file("test.o");
	//generate_c_code("test.o", "test.c"); Under testing and has some bugs....
	getch();
	return 0;
}

