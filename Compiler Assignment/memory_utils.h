

void initializeOpTable()
{
	main_op_table.item = (struct op_item **)malloc(sizeof(struct op_item *) * 100);
	main_op_table.hashTableCount = 0;
	main_op_table.hashTableSize = 100;
	for (int i = 0; i < 100; i++)
	{
		main_op_table.item[i] = NULL;
	}
}

void initialize_symboltable()
{
	main_symbol_table.item = (struct symbol_item **)malloc(sizeof(struct symbol_item *) * 100);
	main_symbol_table.count = 0;
	main_symbol_table.size = 100;
	for (int i = 0; i < 100; i++)
	{
		main_symbol_table.item[i] = NULL;
	}
}

void initialize_instruction_table()
{
	main_intermediate_table.item = (instruction **)malloc(sizeof(instruction) * 1000);
	main_intermediate_table.count = 0;
	main_intermediate_table.size = 1000;
}


void initialize_constant_symboltable()
{
	main_constant_symbol_table.item = (struct constant_symbol_item **)malloc(sizeof(struct constant_symbol_item *) * 100);
	main_constant_symbol_table.count = 0;
	main_constant_symbol_table.size = 100;
	for (int i = 0; i < 100; i++)
	{
		main_constant_symbol_table.item[i] = NULL;
	}
}

void initialize_label_table()
{
	main_label_table.item = (struct label_item **)malloc(sizeof(struct label_item *) * 100);
	main_label_table.count = 0;
	main_label_table.size = 100;
	for (int i = 0; i < 100; i++)
	{
		main_label_table.item[i] = NULL;
	}
}
