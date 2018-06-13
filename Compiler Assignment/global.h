
/*Structure of Opcode table*/
struct op_item {
	char op_name[50];
	int op_code;
	struct op_item *next;
};

struct op_table{
	op_item **item;
	int hashTableCount;
	int hashTableSize;
}main_op_table;

/* End of structure of opcode table */
/* Structure of Symbol Table*/
struct symbol_item{
	char *var_name;
	int start_address;
	int size;
	symbol_item *next;
};

struct symbol_table{
	symbol_item **item;
	int count;
	int size;
}main_symbol_table;

/* End of structure of symbol table */

struct instruction{
	int op_code;
	int *parameters;
	int param_count;
};

struct intermediate_table{
	instruction **item;
	int count;
	int size;
}main_intermediate_table;

struct constant_symbol_item{
	char *var_name;
	int start_address;
	int size;
	constant_symbol_item *next;
};

struct constant_symbol_table{
	constant_symbol_item **item;
	int count;
	int size;
}main_constant_symbol_table;


struct label_item{
	char *label_name;
	int instruction_number;
	label_item *next;
};

struct label_table{
	label_item **item;
	int count;
	int size;
}main_label_table;

struct Stack
{
	int top;
	unsigned capacity;
	int* array;
};


int registers[8];
int memory_blocks[MEMORY_SIZE];
char constant_memory_blocks[MEMORY_SIZE];