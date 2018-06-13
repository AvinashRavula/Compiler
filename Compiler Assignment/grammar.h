
void initialize_optable()
{
	//Allocates memory
	initializeOpTable();
	char grammer[][20] = { "DATA", "CONST", "MOV", "MOV", "ADD", "SUB", "MUL", "JUMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ", "ELSE", "ENDIF","PRINT" };
	//int grammer_len = sizeof(grammer[0]) / sizeof(grammer[0][0]);
	for (int iter = 0; iter < 19; iter++)
		insertIntoOpTable(grammer[iter],iter + 1);
}

char *get_op_name(int index)
{
	char grammer[][20] = { "DATA", "CONST", "MOV", "MOV", "ADD", "SUB", "MUL", "JUMP", "IF", "EQ", "LT", "GT", "LTEQ", "GTEQ", "PRINT", "READ", "ELSE", "ENDIF", "PRINT" };
	char *op_name = (char *)malloc(sizeof(char) * 6);
	strcpy(op_name, grammer[index - 1]);
	return op_name;
}

int get_register_code(char *register_name)
{
	if (strcmp(register_name, "AX") == 0)
		return 0;
	else if (strcmp(register_name, "BX") == 0)
		return 1;
	else if (strcmp(register_name, "CX") == 0)
		return 2;
	else if (strcmp(register_name, "DX") == 0)
		return 3;
	else if (strcmp(register_name, "EX") == 0)
		return 4;
	else if (strcmp(register_name, "FX") == 0)
		return 5;
	else if (strcmp(register_name, "GX") == 0)
		return 6;
	else if (strcmp(register_name, "HX") == 0)
		return 7;
	return -1;
}

