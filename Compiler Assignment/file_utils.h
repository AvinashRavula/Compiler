

int get_size_of_var(char *var_name)
{
	int len = strlen(var_name);
	if (len == 1)
		return 1;
	else if (len > 3)
	{
		int count = 0;
		char **split_var = splitString(var_name, "[", &count);
		if (count > 1)
		{
			return split_var[1][0] - '0';
		}
	}
	return -1;
}

int convert_const_instruction(int op_code, char *sub_instruction, int constant_start_address)
{
	if (strstr(sub_instruction, "="))
	{
		int count = 0;
		char **split_exprsn = splitString(sub_instruction, "=", &count);
		insertIntoSymbolTable(split_exprsn[0], constant_start_address, 0);
		memory_blocks[constant_start_address] = split_exprsn[1][0] - '0';
	}
	else if (strstr(sub_instruction, "["))
	{
		int left_bracket_index = strcspn(sub_instruction, "[");
		int right_bracket_index = strcspn(sub_instruction, "]");
		char *size_of_var = sub_string(sub_instruction, left_bracket_index, right_bracket_index - 1);
		char *var_name = sub_string(sub_instruction, 0, left_bracket_index - 1);
		int var_size = atoi(size_of_var);
		insertIntoSymbolTable(var_name, constant_start_address, 0);
		return constant_start_address + var_size;
	}
	else
		insertIntoSymbolTable(sub_instruction, constant_start_address, 0);
	return constant_start_address + 1;
}

int convert_data_instruction(char *sub_instruction, int start_address)
{
	int size = get_size_of_var(sub_instruction);
	if (size != -1)
	{
		insertIntoSymbolTable(sub_instruction, start_address, size);
		return start_address + size;
	}
	else
		printf("\nproblem at size of var");
}

void convert_if_instruction(char **instruction, int sub_instruction_count,Stack *if_stack, int op_code)
{
	char **params = (char **)malloc(sizeof(char *) * 3);
	for (int iter = 1; iter < sub_instruction_count - 1; iter++)
	{
		params[iter - 1] = instruction[iter];
	}
	push(if_stack, main_intermediate_table.count);
	insert_into_intermediate_table(op_code, params, 3);
}

void convert_else_instruction(int op_code, Stack *if_stack)
{
	int instruction_number = pop(if_stack);
	instruction *item = main_intermediate_table.item[instruction_number];
	item->parameters[item->param_count - 1] = main_intermediate_table.count + 2;
	push(if_stack, main_intermediate_table.count);
	insert_into_intermediate_table(op_code, NULL, 0);
}

void convert_endif_instruction(int op_code, Stack *if_stack)
{
	int instruction_number = pop(if_stack);
	instruction *item = main_intermediate_table.item[instruction_number];
	item->parameters[item->param_count - 1] = main_intermediate_table.count + 2;
	insert_into_intermediate_table(op_code, NULL, 0);
}

void convert_jump_instruction(int op_code, char *sub_instruction)
{	
	char **params = (char **)malloc(sizeof(char *) * 1);
	params[0] = sub_instruction;
	insert_into_intermediate_table(op_code, params, 1);
}

void generate_intermediate_code(char *filename)
{
	if (filename == NULL)
		return;
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL)
	{
		printf("\nUnable to read the source file.");
		return;
	}
	Stack *if_stack = createStack(100);
	char *line = (char *)malloc(sizeof(char) * 500);
	int start_address = 0, constant_start_address = 0;
	while (!feof(fptr))
	{
		fgets(line, 500, fptr);
		trim(line);
		int colon_index = strcspn(line,":");
		int line_length = strlen(line);
		if (colon_index == line_length - 1)
		{
			//JUMP Label found
			insertIntoLabelTable(sub_string(line, 0, colon_index - 1),main_intermediate_table.count + 1);
			continue;
		}

		int split_instr_count = 0;
		char **split_instruction = splitString(line, " ", &split_instr_count); 
		if(split_instruction[0][0] == '\t')
			split_instruction[0] = sub_string(split_instruction[0],1,strlen(split_instruction[0]));

		op_item *cur_op_item = searchInOpTable(split_instruction[0]);
		if (cur_op_item != NULL && split_instruction != NULL){
			trim(split_instruction[1]);
			switch (cur_op_item->op_code)
			{
			case 1:
				start_address = convert_data_instruction(split_instruction[1],start_address);
				break;
			case 2:
				start_address = convert_const_instruction(cur_op_item->op_code, split_instruction[1], start_address);
				break;
			case 8:
				convert_jump_instruction(cur_op_item->op_code,split_instruction[1]);
				break;
			case 9:
				convert_if_instruction(split_instruction,split_instr_count,if_stack,cur_op_item->op_code);
				break;
			case 17://Else Block
				convert_else_instruction(cur_op_item->op_code,if_stack);
				break;
			case 18:
				convert_endif_instruction(cur_op_item->op_code,if_stack);
				break;
			default:
				int count = 0;
				char **splitted_params = splitString(split_instruction[1], ",", &count);
				insert_into_intermediate_table(cur_op_item->op_code, splitted_params, count);
				break;
			}
		}
	}
	fclose(fptr);
}


void generate_intermediate_code_file(char *source_file, char *destination_file)
{
	generate_intermediate_code(source_file);
	FILE *fptr = fopen(destination_file, "w");
	if (fptr != NULL)
	{
		for (int i = 0; i < main_symbol_table.size; i++)
		{
			if (main_symbol_table.item[i] != NULL){
				fprintf(fptr, "%s %d %d", main_symbol_table.item[i]->var_name, main_symbol_table.item[i]->start_address, main_symbol_table.item[i]->size);
					
				if (main_symbol_table.item[i]->size == 0)
					fprintf(fptr, " %d", memory_blocks[main_symbol_table.item[i]->start_address]);
				fprintf(fptr, "\n");
				symbol_item *item = main_symbol_table.item[i]->next;
				while (item != NULL)
				{
					fprintf(fptr, "%s %d %d", item->var_name, item->start_address, item->size);
					if (item->size == 0)
						fprintf(fptr, " %d", memory_blocks[main_symbol_table.item[i]->start_address]);
					fprintf(fptr, "\n");
					item = item->next;
				}
			}
		}
		fprintf(fptr,"\n");

		for (int i = 0; i<main_label_table.size; i++) {
			if (main_label_table.item[i] != NULL){
				fprintf(fptr,"%s %d\n", main_label_table.item[i]->label_name, main_label_table.item[i]->instruction_number);
				label_item *item = main_label_table.item[i]->next;
				while (item != NULL)
				{
					fprintf(fptr, "%s %d\n", item->label_name, item->instruction_number);
					item = item->next;
				}
			}
		}
		fprintf(fptr, "\n");
		

		for (int iter = 0; iter < main_intermediate_table.count; iter++)
		{
			fprintf(fptr,"%d", main_intermediate_table.item[iter]->op_code);
			for (int iter2 = 0; iter2 < main_intermediate_table.item[iter]->param_count; iter2++)
				fprintf(fptr, " %d", main_intermediate_table.item[iter]->parameters[iter2]);
			fprintf(fptr, "\n");
		}
		fclose(fptr);
	}
}

void load_intermediate_code_file(char *filename)
{
	FILE *fptr = fopen(filename, "r");
	if (fptr != NULL)
	{
		char *line = (char *)malloc(sizeof(char));
		int table = 1;
		while (!feof(fptr))
		{
			fgets(line, 500, fptr);
			int count = 0;
			char **split = splitString(line, " ", &count);
			if (line[0] == '\n')
			{
				table++;
				continue;
			}
			int len = strlen(split[count - 1]);
			if (split[count - 1][len - 1] == '\n')
				split[count - 1][len - 1] = '\0';
			if (table == 1)
			{
				if (count == 3 || count == 4){
					insertIntoSymbolTable(split[0], atoi(split[1]), atoi(split[2]));
					if (count == 4)
						memory_blocks[atoi(split[1])] = atoi(split[3]);
				}
			}
			else if (table == 2)
			{
				if (count == 2)
					insertIntoLabelTable(split[0], atoi(split[1]));
			}
			else{
				int *arr = (int *)malloc(sizeof(int) * (count - 1));
				for (int i = 0; i < count - 1; i++)
				{
					arr[i] = atoi(split[i + 1]);
				}
				insert_into_intermediate_table_without_validations(atoi(split[0]), arr, count - 1);
			}
		}
		fclose(fptr);
	}
}

char *get_symbol_name(int startAddress)
{
	for (int i = 0; i < main_symbol_table.size; i++)
	{
		if (main_symbol_table.item[i] != NULL){
			if (main_symbol_table.item[i]->start_address == startAddress)
				return main_symbol_table.item[i]->var_name;
			symbol_item *item = main_symbol_table.item[i]->next;
			while (item != NULL)
			{
				if (item->start_address == startAddress)
					return main_symbol_table.item[i]->var_name;
				item = item->next;
			}
		}
	}
}

void generate_c_code(char *source_file, char *destination_file)
{
	load_intermediate_code_file(source_file);
	FILE *fptr = fopen(destination_file, "w");
	if (fptr != NULL)
	{
		char labels[20][50];
		int label_indexes[20];
		int index = 0;
		fprintf(fptr, "#include<stdio.h>\n\nint registers[8];\nvoid main()\n{");
		for (int i = 0; i < main_symbol_table.size; i++)
		{
			if (main_symbol_table.item[i] != NULL){
				if (main_symbol_table.item[i]->size > 1)
					fprintf(fptr, "\tint %s[%d];\n", main_symbol_table.item[i]->var_name, main_symbol_table.item[i]->size);
				else if (main_symbol_table.item[i]->size == 1)
					fprintf(fptr, "\tint %s;\n", main_symbol_table.item[i]->var_name);
				else if (main_symbol_table.item[i]->size == 0)
					fprintf(fptr, "\tconst int %s = %d;\n", main_symbol_table.item[i]->var_name, memory_blocks[main_symbol_table.item[i]->start_address]);

				symbol_item *item = main_symbol_table.item[i]->next;
				while (item != NULL)
				{
					if (item->size > 1)
						fprintf(fptr, "\tint %s[%d];\n",item->var_name, item->size);
					else if (item->size == 1)
						fprintf(fptr, "\tint %s;\n", item->var_name);
					else if (item->size == 0)
						fprintf(fptr, "\tconst int %s = %d;\n", item->var_name,memory_blocks[item->start_address]);
					item = item->next;
				}
			}
		}
		fprintf(fptr, "\n");

		
		for (int i = 0; i<main_label_table.size; i++) {
			if (main_label_table.item[i] != NULL){
				//fprintf(fptr, "%s %d\n", main_label_table.item[i]->label_name, main_label_table.item[i]->instruction_number);
				strcpy(labels[index], main_label_table.item[i]->label_name);
				label_indexes[index++] = main_label_table.item[i]->instruction_number;
				label_item *item = main_label_table.item[i]->next;
				while (item != NULL)
				{
					strcpy(labels[index], item->label_name);
					label_indexes[index++] = item->instruction_number;
					item = item->next;
				}
			}
		}
		fprintf(fptr, "\n");

		char *name;
		for (int iter = 0; iter < main_intermediate_table.count; iter++)
		{
			instruction *item = main_intermediate_table.item[iter];
			for (int i = 0; i < index; i++)
			{
				if (iter == label_indexes[i] - 1)
				{
					fprintf(fptr, "%s:\n", labels[i]);
					continue;
				}
			}
			switch (main_intermediate_table.item[iter]->op_code)
			{
			case 3:

				break;
			case 4:
				name = get_symbol_name(item->parameters[0]);
				fprintf(fptr, "%s = registers[%d];\n", name , item->parameters[1]);
				break;
			case 5:
				fprintf(fptr, "registers[%d] = registers[%d] + registers[%d];\n", item->parameters[0], item->parameters[1], item->parameters[2]);
				break;
			case 6:
				fprintf(fptr, "registers[%d] = registers[%d] - registers[%d];\n", item->parameters[0], item->parameters[1], item->parameters[2]);
				break;
			case 8:
				for (int x = 0; x < index; x++){
					if (label_indexes[x] == item->parameters[0]){
						fprintf(fptr, "GOTO %s;\n", "%d", labels[x]);
						break;
					}
				}
				break;
			case 9:
				fprintf(fptr, "if (registers[%d] %s registers[%d])\n{\n", item->parameters[0], get_op_name(item->parameters[1]), item->parameters[2]);
				break;
			case 16:
				fprintf(fptr, "scanf(\"%s\",&registers[%d];\n", "%d", main_intermediate_table.item[iter]->parameters[0]);
				break;
			case 17:
				fprintf(fptr, "}\nelse{\n");
				break;
			case 18:
				fprintf(fptr, "}\n");
				break;
			case 19:
				fprintf(fptr, "printf(\"%s\",registers[%d];\n", "%d", item->parameters[0]);
				break;
			}

			fprintf(fptr, "%d", main_intermediate_table.item[iter]->op_code);
			for (int iter2 = 0; iter2 < main_intermediate_table.item[iter]->param_count; iter2++)
				fprintf(fptr, " %d", main_intermediate_table.item[iter]->parameters[iter2]);
			fprintf(fptr, "\n");
		}
		fclose(fptr);

		/*
		char labels[20][20];
		int label_indexes[20]; 
		fprintf(write_fptr, "#include<stdio.h>\n\nvoid main()\n{");
		char *line = (char *)malloc(sizeof(char));
		int table = 1, iter = -1, index = 0;

		while (!feof(fptr))
		{
			fgets(line, 500, fptr);
			int count = 0;
			char **split = splitString(line, " ", &count);
			if (line[0] == '\n')
			{
				table++;
				continue;
			}
			int len = strlen(split[count - 1]);
			if (split[count - 1][len - 1] == '\n')
				split[count - 1][len - 1] = '\0';
			if (table == 1)
			{
				if (count == 3){
					if (atoi(split[2]) > 1)
						fprintf(write_fptr, "\tint %s[%s];\n", split[0],split[2]);
					else
						fprintf(write_fptr, "\tint %s;\n",split[0]);
				}
				else if (count == 4){
					fprintf(write_fptr, "\tconst int %s;\n", split[0]);
				}
			}
			else if (table == 2)
			{
				if (strcmp(split[0], "START") == 0)
					continue;
				if (count == 2){
					strcpy(labels[index], split[0]);
					label_indexes[index++] = atoi(split[1]);
				}
			}
			else{
				if (iter == -1)
					iter = 0;
				for (int i = 0; i < index; i++)
				{
					if (iter == label_indexes[i])
						fprintf(write_fptr, "%s:\n", labels[i]);
				}
				switch (atoi(split[0]))
				{
				case 16:
					fprintf(write_fptr, "scanf(\"%d\",&%s);\n",)
				}
			}
		}
		fclose(fptr);*/
	}
}