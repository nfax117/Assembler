Program to be an assembler for a custom assembly language. The assembly language has instructions, addressing modes, and data declarations. The code reads an input file containing assembly code and translates it into machine code, writing the result to an output file.

1.) Data Structures:
- Three dictionaries (Instructions, AddressingModes, and others like InstructionsSpec) are defined to map instruction mnemonics, addressing modes, and special cases to their corresponding machine code representations.
- Labels is a map to store labels and their corresponding memory addresses.

2.) Reading Labels:
- The code reads the input file line by line.
- If a line starts with a lowercase letter or a digit, it is considered a label. The label is stored in the Labels map with its corresponding memory address.

3.) Generating Machine Code:
- The code then goes through the lines again, generating machine code for instructions and data declarations.
- It handles various cases based on the number of arguments and whether an addressing mode is present.
- It calculates memory addresses for labels and updates the machine code accordingly.
- It handles special cases for certain instructions and addressing modes.

4.) Output:
- The generated machine code is written to the output file.

5.) Error Handling:
- The code includes some basic error handling. For example, it checks for the definition of labels multiple times and reports an error if a label is used but not defined.

6.) Main Function:
- The main function checks if the correct number of command-line arguments is provided and then calls the assembly function with the input and output file paths.
