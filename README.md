51assemble
==========

        This is a very simple assembler for 51 serials microcontrollers，and it's also the first complete project I handled by 
    my own.
        Thanks to my classmates -- Deng and Gou, they gave me so much help, and then I could get ideas to the project, 
    sincerely thankyou so much.
        The project contains only one file, that is "51assemble.cpp"，and I'll optimize it later in the rest of this month.

    Specification of program:
        Here is the perhaps the procedures of this program, first of all, read the source file line by line, and you'll get the
    instructions, then you should separate the instruction into Label，mnemonic, the first operand, the second operand, the 
    third operand, and store them into character string, and match the instruction with every line of the "HEX.txt", when you 
    search the instruction, you get the machine code of that instruction and the present address, also, you should get the 
    label-form and Pseudo instruction form, then you just need to output your data, but you should save your data in character 
    string first, and write the string into "obj.txt".


    程序说明：
    
        大概思路是这样的，用一个循环逐行读取源文件data.txt的每一行指令，将读取到的指令
    以标号，助记符，第一操作数，第二操作数，第三操作数等分别拆开，储存，然后与做好的指
    令对照表匹配获取机器码，当前地址，在得到当前地址的同时建立标号表，对于扫描到的伪指
    令建立伪指令表，然后是输出，以字符串储存要输出的一行数据，将字符串写入obj.txt即可。
    
        程序中大量用到16进制字符串转化为整数，整数转为16进制字符串等操作，所以，定义专
    门的转换函数很有必要，而我现在也正在为这个发愁，没有完美的实现string_to_int的功能，
    继续优化中。




==========================================================================================

    change log：
    
    2014/09/17
    
    1.Submit the first alpha version;
    2.The program is not fully completed, still thinking of function algorithm;
    3.Submit "data.txt" and "HEX.txt";
