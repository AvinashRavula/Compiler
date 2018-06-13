

void insert_into_intermediate_table(int op_code, char **params, int param_count)
{
	// MOV at 3 is the MOV AX,A and MOV at 4 is MOV A,AX

	instruction *new_instruction = (instruction *)malloc(sizeof(instruction));
	new_instruction->op_code = op_code;
	new_instruction->param_count = param_count;
	label_item *item;
	switch (op_code)
	{
	case 9:
	case 17:// If and else 
		new_instruction->parameters = (int *)malloc(sizeof(int) * param_count + 1);
		new_instruction->param_count = param_count + 1;
		//As of now, the address of the else block is unknown, so storing -1 to indicate unknown.
		new_instruction->parameters[new_instruction->param_count - 1] = -1;
		break;
	case 8:// JUMP
		new_instruction->parameters = (int *)malloc(sizeof(int) * param_count);
		item = searchInLabelTable(params[0]);
		if (item != NULL)
		{
			new_instruction->parameters[0] = item->instruction_number;
		}
		param_count = 0;//To skip the loop 
		break;
	default:
		new_instruction->parameters = (int *)malloc(sizeof(int) * param_count);
	}
	for (int iter = 0; iter < param_count; iter++)
	{
		trim(params[iter]);
		int code = -1;

		if (strlen(params[iter]) == 2)
			code = get_register_code(params[iter]);
		if (code >= 0){
			new_instruction->parameters[iter] = code;
			if (op_code == 4 && iter == 0)
				new_instruction->op_code = op_code - 1;
			if (op_code == 19) // PRINT refers to registers if register is given else memory.
				new_instruction->op_code = 15;
		}
		else if (op_code == 9) // Operation is IF, it contains only register operations
		{
			op_item *item = searchInOpTable(params[iter]);
			if (item != NULL)
			{
				new_instruction->parameters[iter] = item->op_code;
			}
		}
		else
		{
			int index = strcspn(params[iter], "[");
			if (index != strlen(params[iter]))
			{
				//It is array
				int endIndex = strcspn(params[iter], "]");
				int address = atoi(sub_string(params[iter], index + 1, endIndex - 1));
				char *var = sub_string(params[iter], 0, index - 1);
				symbol_item *item = searchInSymbolTable(var);
				if (item != NULL)
					new_instruction->parameters[iter] = item->start_address + address;
				else
				{
					constant_symbol_item *item = searchInConstantSymbolTable(var);
					if (item != NULL){
						if (iter != 0)
							new_instruction->parameters[iter] = item->start_address + address;
						else{
							printf("Error : trying to modify constant variable");
							return;
						}
					}
				}
			}
			else
			{
				symbol_item *item = searchInSymbolTable(params[iter]);
				if (item != NULL)
					new_instruction->parameters[iter] = item->start_address;
				else {
					constant_symbol_item *item = searchInConstantSymbolTable(params[iter]);
					if (item != NULL)
						new_instruction->parameters[iter] = item->start_address;
				}
			}
		}			
	}
	main_intermediate_table.item[main_intermediate_table.count] = new_instruction;
	main_intermediate_table.count += 1;
}

void insert_into_intermediate_table_without_validations(int op_code, int *params, int param_count)
{
	instruction *new_instruction = (instruction *)malloc(sizeof(instruction));
	new_instruction->op_code = op_code;
	new_instruction->param_count = param_count;
	new_instruction->parameters = (int*)malloc(sizeof(int) * param_count);
	for (int i = 0; i < param_count; i++)
	{
		new_instruction->parameters[i] = params[i];
	}
	main_intermediate_table.item[main_intermediate_table.count] = new_instruction;
	main_intermediate_table.count += 1;
}

void display_intermediate_table()
{
	for (int iter = 0; iter < main_intermediate_table.count; iter++)
	{
		printf("\n%5d %5d ", iter + 1, main_intermediate_table.item[iter]->op_code);
		for (int iter2 = 0; iter2 < main_intermediate_table.item[iter]->param_count; iter2++)
			printf("%5d ", main_intermediate_table.item[iter]->parameters[iter2]);
	}
}