#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;
class Inst
{
private:
	int machcode;		//机器码
	int width;			//字节数
	string label;		//标号
	string op_mne;		//助记符
	string op_num[3];	//操作数

public:
	Inst()				//构造函数初始化
	{
		machcode=-1;
		width=0;
		label="\0";
		op_mne="\0";
		string op_num[3]={"\0","\0","\0"};
	}
	~Inst()	
	{
	}
	int get_machcode(string *s);				//获取机器码
	string get_mne(string s);				//获取助记符
	string get_op_num(string s, int i);		//获取操作数
	string get_label(string s);				//获取标号
	int get_addr(string s);				//获取当前指令地址
	friend void get_obj(string s);		//输出hex文件
	string match(string s);								//组合字符串
	string int_to_string(int t,int n);				//整数转换成字符串
	string get_checksum(string s);					//获得校验码
};
static int addr=0;				//地址
static string mc;				//机器码字符串
static string cs;					//用来获取校验和的字符串

void readasm(Inst inst);						//读取源程序


int Inst::get_machcode(string *s)
{
	//获取改行指令对应的机器码
	ifstream in("HEX.txt");
	string filename;
	string str,str1,str2,str3,st[5],sc[5],la[2];
	//str是读取出HEX.txt的一行完整指令，不允许破坏
	//str1储存机器码部分
	//str2储存字节宽度
	//st[5]储存操作数
	//sc[5]复制s[5]，防止s[5]破坏
	//la[2]存储伪指令和其对应的地址
	stringstream ss;
	for(int n=0;n<5;n++)
		sc[n]=s[n];
	st[0]="\0";

	if(in) // 有该文件
	{
		while (getline (in, str)) // str中不包括每行的换行符
		{
			//首先判断伪指令
			if(sc[1]=="ORG")
			{
				sc[2]=s[2].substr(0,s[2].length()-1);
				addr=atoi(sc[2].c_str());
				la[0]=sc[1];
				la[1]=sc[2];
				
			}
			else if(sc[1]=="END")
			{
				la[0]=sc[1];
				la[1]=int_to_string(addr,4);					//整型数转字符串
				
			}
			else
			{

				str2=str.substr(str.find(":")+1,1);								//截断读取到的一行字符串中字节长度部分
				str1=str.substr(0,2);													//截断读取到的一行字符串中机器码部分
				str3=str.substr(str.find(":")+1,str.length());					//其余部分
				//	cout<<str<<endl;												//检验
				str3=str3.substr(str3.find(":")+1,str3.length());
				//cout<<str<<endl;													//检验

				//将str1，str2，即机器码和字节长度由字符串数组转换为十进制数字
				const char *t = str1.data();
				const char *w=str2.data();
				int i=0, tmp, result1=0,result2=0;
				for(i=0; i<strlen(t); i++) /* 把字符一个一个转成16进制数 */
				{
					if((t[i]>='0')&&(t[i]<='9'))
						tmp = t[i]-'0';
					else if((t[i]>='A')&&(t[i]<='F'))
						tmp = t[i]-'A'+10;
					else if((t[i]>='a')&&(t[i]<='f'))
						tmp = t[i]-'a'+10;
					else
						return -1;  /* 出错了 */
					result1 = result1*16+tmp;  /* 转成16进制数后加起来 */
				}
				tmp=w[0]-'0';
				result2 = result2*16+tmp;  /* 转成16进制数后加起来 */
				machcode=result1;							//机器码
				width=result2;									//字节长度
				//cout<<t<<"\t"<<result1<<"\t"<<result2<<endl;							//检验result
				
				//匹配指令，返回机器码
				st[1]=str3.substr(0,str3.find(":"));
				str3=str3.substr(str3.find(":")+1,str3.length());

				if(sc[2]!="\0")																						//至少有一个操作数
				{
					st[2]=str3.substr(0,str3.find(":"));
					str3=str3.substr(str3.find(":")+1,str3.length());
					if(sc[3]!="\0")																					//至少有两个操作数
					{
						st[3]=str3.substr(0,str3.find(":"));
						str3=str3.substr(str3.find(":")+1,str3.length());
						if(sc[4]!="\0")																				//有三个操作数
						{
							st[4]=str3.substr(0,str3.find(":"));
							str3=str3.substr(str3.find(":")+1,str3.length());
							if((st[1]==sc[1])&&(st[2]==sc[2])&&(st[3]==sc[3])&&(st[4]==sc[4]))		//匹配
							{

								//建立标号表
								if(sc[0]!="\0")
								{
									la[0]=sc[0];
									la[1]=int_to_string(addr,4);			//整型数转字符串
									//cout<<la[0]<<"\t"<<la[1]<<endl;
								}
								else
								{}

								//get_obj();
								//	cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;		//输出匹配的指令的机器码和字节长度以及当前地址
								//cout<<sc[0]<<endl;
								addr+=width;
								mc+=str1;
								//cout<<mc<<endl;				//检验机器码串
								return machcode;
							}
							else													//	不匹配
							{}
						}
						else																							//有两个操作数
						{
							if((st[1]==sc[1])&&(st[2]==sc[2])&&(st[3]==sc[3]))							//匹配
							{	
								//建立标号表
								if(sc[0]!="\0")
								{
									la[0]=sc[0];
									la[1]=int_to_string(addr,4);					//整型数转字符串
									
							//		cout<<la[0]<<"\t"<<la[1]<<endl;
								}
								else
								{}

								//get_obj();
						//		cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;	//输出匹配的指令的机器码和字节长度以及当前地址
								//cout<<sc[0]<<endl;
								addr+=width;
								mc+=str1;
								//cout<<mc<<endl;				//检验机器码串
								return machcode;
							}
							else													//	不匹配
							{}
						}
					}
					else																							//只有一个操作数
					{
						if((st[1]==sc[1])&&st[2]==sc[2])				//匹配
						{

							//建立标号表
							if(sc[0]!="\0")
							{
								la[0]=sc[0];
								la[1]=int_to_string(addr,4);					//整型数转字符串
								
					//		cout<<la[0]<<"\t"<<la[1]<<endl;
							}
							else
							{}

							//get_obj();
					//	cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;	//输出匹配的指令的机器码和字节长度以及当前地址
							//	cout<<sc[0]<<endl;
							addr+=width;
							mc+=str1;
							//cout<<mc<<endl;				//检验机器码串
							return machcode;
						}
						else													//	不匹配
						{}
					}
				}
				else																								//没有操作数
				{
					if(st[1]==sc[1])										//匹配									
					{					

						//建立标号表
						if(sc[0]!="\0")
						{
							la[0]=sc[0];
							la[1]=int_to_string(addr,4);					//整型数转字符串
							
				//		cout<<la[0]<<"\t"<<la[1]<<endl;
						}
						else
						{}

						//get_obj();
			//		cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;	//输出匹配的指令的机器码和字节长度以及当前地址
						//	cout<<sc[0]<<endl;
						addr+=width;
						mc+=str1;
						//cout<<mc<<endl;				//检验机器码串
						return machcode;
					}
					else														//不匹配
					{}
				}

			}
		}

		//cout<<la[0]<<"\t"<<la[1]<<endl;				//输出伪指令表

		//源文件中没有匹配的指令
		//for(int i=0;i<=4;i++)
		//{
		//	cout<<s[i]<<"\t";
		//}
		//cout<<"这条不匹配"<<endl;

	}//while结束

	else // 没有该文件
	{
		cout << "打开文件" << filename << "出错！" << endl;
		return -1;
	}

}//get_machcode结束

string Inst::get_mne(string s)
{
	//获取助记符
	int i;
	i=s.find(" ");
	if(i!=-1)
	{
		op_mne=s.substr(0,i);
		//cout<<"助记符:"<<op_mne<<endl;							//输出助记符
		return op_mne;
	}
	else
	{
		op_mne=s;
		//cout<<"助记符:"<<op_mne<<endl;							//输出助记符
		return op_mne;

	}
}


string Inst::get_op_num(string s, int i)
{
	int j;
	j=s.find(",");
	if(j!=-1)
	{
		op_num[i]=s.substr(0,j);
		//cout<<"操作数"<<i+1<<":"<<op_num[i]<<endl;					//输出操作数
		return op_num[i];	
	}
	else
	{

		//op_num[i]=s.substr(s.find(" "),s.length());
		op_num[i]=s;
		//cout<<"操作数"<<i+1<<":"<<op_num[i]<<endl;					//输出操作数
		return op_num[i];
	}
}

string Inst::get_label(string s)
{
	int i;
	i=s.find(":");
	label=s.substr(0,i);
	//	cout<<"标号:"<<label<<endl;
	return label;

}


void readasm(Inst inst)
{
	string filename = "data.txt";					//源文件
	ifstream fin(filename.c_str());  
	if( !fin )													//防错
	{   
		cout << "打开文件" << filename << "出错！" << endl;   
		exit(-1);
	}
	string str,s[5],sn[5],tmp[5];						//str储存读取的每行字符串，s[]保存截断的每部分
	int i;
	while(std::getline(fin,str))
	{

		if(str.find(":")!=-1)								//如果有标号
		{
			s[0]=inst.get_label(str);							//获取标号
			str=str.substr(str.find(":")+1,str.length());	//截取":"后的字符串
		}
		else
		{
			s[0]="\0";
		}
		s[1]=inst.get_mne(str);									//先获取助记符
		if(str.find(" ")!=-1)											//如果有操作数
		{
			str=str.substr(str.find(" ")+1,str.length());	//截取" "后的字符串
			if(str.find(",")!=-1)										//如果有不止一个操作数
			{
				s[2]=inst.get_op_num(str,0);						//获取第一操作数
				str=str.substr(str.find(",")+1,str.length());	//截取","后的字符串
				if(str.find(",")!=-1)										//如果有三个操作数
				{
					s[3]=inst.get_op_num(str,1);					//获取第二操作数
					str=str.substr(str.find(",")+1,str.length());	//截取","后的字符串
					s[4]=inst.get_op_num(str,2);						//获取第三操作数
				}
				else
				{
					s[4]="\0";
					s[3]=inst.get_op_num(str,1);						//获取第二操作数

				}
			}
			else
			{
				s[3]="\0";
				s[4]="\0";
				s[2]=inst.get_op_num(str,0);							//获取第一操作数
			}
		}
		else
		{
			s[2]="\0";
			s[3]="\0";
			s[4]="\0";
		}
		for(i=0;i<=4;i++)
			sn[i]=s[i];
		for(i=2;i<=4;i++)
		{
			if(sn[i]!="\0")
			{
				//将地址转换为arg，方便匹配HEX.txt内的每一行指令
				if((sn[i]=="A")||(sn[i]=="B")||(sn[i]=="C")||(sn[i]=="AB")||(sn[i]=="DPTR")||(sn[i][0]=='R'))
				{}
				else if((sn[i][0]=='@')||(sn[i][0]=='/'))
				{}
				else if((sn[1]=="ORG")||(sn[1]=="END"))
				{}
				else
				{
					if(sn[i][0]=='#')
					{
						sn[i]="#arg";
					}
					else if(sn[i][sn[i].length()-1]=='H')
					{
						sn[i]="arg";
					}
					else
					{
						sn[i]="arg";
					}
				}
			}
			else
			{}

		}

		//for(i=0;i<=4;i++)						//检验sn[]
		//{
		//		cout<<sn[i]<<"\t";
		//}
		//cout<<endl;
		for(i=0;i<=4;i++)
		{
			tmp[i]=sn[i];
			sn[i]=s[i];
			s[i]=tmp[i];
		}
		//for(i=0;i<=4;i++)					//检验s[]
		//{
		//	cout<<s[i]<<"\t";
		//}
		//cout<<endl;

		inst.get_machcode(s);
	}

}

string Inst::int_to_string(int t, int n)					//将整数转为字符串，可以设置任意长度，方便地址和机器码两种长度
{
	stringstream ss;
	string str;
	ss<<hex<<t;
	ss>>str;
	for(int i=1;i<=n;i++)
	{
		if(str.length()<n)
		{
			str="0"+str;
		}
		else if(str.length()>n)
		{
			cout<<"error"<<endl;
			exit(-1);
		}
		else
		{}
	}
	transform(str.begin(), str.end(), str.begin(), toupper);				//转换为大写
//	cout<<str<<endl;
	return str;
}

string Inst::match(string s)
{
	string sen,tmp;
	int w;
	w=s.length();
	tmp=int_to_string(w,2);
	sen.append(tmp);
	tmp=int_to_string(addr,4);
	sen.append(tmp);
	sen.append("00");
	//sen.append(mc);
	//cout<<sen<<endl;								//检验前段8个字符
	return sen;
}

string Inst::get_checksum(string s)			//校验和
{
	int n,i=0;
	string s1,s2,str[100];
	s1=int_to_string(	s.length(),2);
	s2=int_to_string(addr,4);
	s=s1+s2+"00"+s;
	n=s.length();
	//	cout<<s.substr(0,2)<<"\t"<<n<<endl;
	//cout<<mc<<endl;					//检验机器码串
	for(i=0;i<(n/2);i++)
	{
		str[i]=s.substr(0,2);
		//cout<<s.substr(0,2);				//检验截断情况
		s=s.substr(2,s.length());
	//	cout<<str[i]<<endl;
	}
		return cs;
}


void get_obj(string s)		//输出hex文档
{
	ofstream in;
	Inst inst;
	int l;
	string str;
	l=s.length();
	//inst.get_checksum(cs);
	in.open("obj.txt",ios::app); //ios::trunc表示在打开文件前将文件清空，app是接上，由于是写入,文件不存在则创建
	in<<":"<<inst.match(s)<<mc<<inst.get_checksum(mc)<<"\n";
	in<<":00000001FF"<<"\n";
	//in<<":"<<setw(2) <<setfill('0')<<setiosflags(ios::uppercase)<<hex<<machcode<<dec<<addr<<endl;
	in.close();//关闭文件

}

FILE *fp_in;
FILE *fp_out;

//const char *fake[]={	"ORG","END","DB","EQU","DB","DW","DS","BIT"	};			//伪指令


int main()
{
	Inst inst;
	readasm(inst);
	//inst.int_to_string(9999,5);
	//inst.get_checksum(mc);
	get_obj(mc);
	return 1;
}
