
bool condition(int instruction_pointer)
{
	instruction *item = main_intermediate_table.item[instruction_pointer];
	int value1 = registers[item->parameters[0]];
	int value2 = registers[item->parameters[2]];
	int cond = item->parameters[1];
	switch (cond)
	{
	case 10:
		return value1 == value2;
		break;
	case 11:
		return value1 < value2;
		break;
	case 12:
		return value1 > value2;
		break;
	case 13:
		return value1 <= value2;
		break;
	case  14:
		return value1 >= value2;
		break;
	}
	return false;
}


void execute_intermediate_code()
{
	instruction *item;
	for (int iter = 0; iter < main_intermediate_table.count; iter++)
	{
		item = main_intermediate_table.item[iter];
		switch (main_intermediate_table.item[iter]->op_code)
		{
		case 3:
			//MOV memory to registers
			registers[item->parameters[0]] = memory_blocks[item->parameters[1]];
			break;
		case 4:
			//MOV registers to memory
			memory_blocks[item->parameters[0]] = registers[item->parameters[1]];
			break;
		case 5:
			//ADD
			registers[item->parameters[0]] = registers[item->parameters[1]] + registers[item->parameters[2]];
			break;
		case 6:
			//SUB
			registers[item->parameters[0]] = registers[item->parameters[1]] - registers[item->parameters[2]];
			break;
		case 7:
			registers[item->parameters[0]] = registers[item->parameters[1]] * registers[item->parameters[2]];
			break;
		case 8:
			//JUMP
			iter = item->parameters[0] - 2;
			break;
		case 9:
			//IF
			if (!condition(iter))
			{
				iter = item->parameters[3] - 2;
			}
			break;
		case 15:
			//PRINT
			printf("%d", registers[item->parameters[0]]);
			break;
		case 16:
			//READ
			int ch;
			fflush(stdin);
			scanf("%d", &ch);
			registers[item->parameters[0]] = ch;
			break;
		case 17:
			//ELSE
			iter = item->parameters[0] - 2;
			break;
		case 19:
			//PRINT
			printf("%d", memory_blocks[item->parameters[0]]);
			break;
		}
	}
}