ast: Abstract_Syntax_tree.c parameter_list.c built_in_functions.c
	gcc -o ast Abstract_Syntax_tree.c parameter_list.c built_in_functions.c

tokenizer: tokenizer.c
	gcc -o tokenizer tokenizer.c
 
 parser: parser.c tokenizer.c Abstract_Syntax_tree.c parameter_list.c built_in_functions.c tokenizer.c
	gcc -o parser parser.c tokenizer.c Abstract_Syntax_tree.c parameter_list.c built_in_functions.c