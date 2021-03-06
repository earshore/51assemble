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
	int machcode;				//机器码
	int width;						//字节数
	string label;					//标号
	string op_mne;				//助记符
	string op_num[3];			//操作数
public:
	Inst()								//构造函数初始化
	{
		machcode=-1;
		width=0;
		label="\0";
		op_mne="\0";
		string op_num[3]={"\0","\0","\0"};
	}
	~Inst()	{}
	int get_machcode(string *s);							//获取机器码
	string get_mne(string s);								//获取助记符
	string get_op_num(string s, int i);					//获取操作数
	inline string get_label(string s);						//获取标号
	friend void get_obj(string se);						//输出hex文件
	string match(string dataline);						//组合字符串
	string int_to_string(int t,int n);						//整数转换成字符串
	int string_to_int(string s);								//字符串转整数
	string get_checksum(string dataline);			//获得校验码
};

static int addr=0;						//地址
static int addr_s=0;					//起始地址，可用于输出文档时
static int lnum=0;						//指令行数 
static int laddr[100];					//每行指令所在地址
static string firsta[100];				//第一次汇编得到字符串
static string datal="\0";			//数据字符串
static string sn[5];						//记录源文件每行指令的每个部分
static int num=0;						//记录标号表序号
static string sa[100];					//标号表存储标号
static int sad[100];					//标号表储存地址

static int lab_addr[100];			//用于建立标号表时储存标号代表的地址
void readasm(Inst inst);				//读取源程序
string replacelab(string dataline, const string oldc, const string newc);		//替换第一个目标字符串

int Inst::get_machcode(string *s)											//获取指令对应的机器码
{
	ifstream in("HEX.txt");
	string filename;
	string str,str1,str2,str3,st[5],sc[5];
	//str存储读取出HEX.txt的一行完整指令，不允许破坏
	//str1储存机器码部分
	//str2储存字节宽度
	//st[5]储存操作数
	//sc[5]复制s[5]，防止s[5]破坏
	//la[2]存储伪指令和其对应的地址
	for(int n=0;n<5;n++)
		sc[n]=s[n];
	st[0]="\0";
	if(in) // 有该文件
	{
		while (getline (in, str))													// str中不包括每行的换行符
		{
			//首先判断伪指令
			if(sc[1]=="ORG")														//如果是，储存伪指令及其地址
			{
				sc[2]=s[2].substr(0,s[2].length()-1);			
				addr_s=string_to_int(sc[2]);
				addr=addr_s;
				laddr[lnum]=addr;
			}
			else if(sc[1]=="END")	{}
			else	{}
			str2=str.substr(str.find(":")+1,1);								//截断读取到的一行字符串中字节长度部分
			str1=str.substr(0,2);													//截断读取到的一行字符串中机器码部分
			str3=str.substr(str.find(":")+1,str.length());					//其余部分
			str3=str3.substr(str3.find(":")+1,str3.length());

			//将str1，str2，即机器码和字节长度由十六进制字符串数组转换为十进制数字
			int result1=0,result2=0;
			result1=string_to_int(str1);
			result2=string_to_int(str2);
			machcode=result1;																				//机器码
			width=result2;																						//字节长度
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
						{firsta[lnum]=str1;
						//建立标号表
						if(sc[0]!="\0")
						{
							lab_addr[lnum]=addr;
							sa[num]=sc[0];
							sad[num]=addr;
							num++;
						}
						else	{}
						for(int i=2;i<=4;i++)
						{
							if(sc[i].find("arg")!=-1)
							{
								if((sn[i][0]=='#')&&(sn[i][sn[i].length()-1]=='H'))
								{
									str1+=sn[i].substr(1,sn[i].length()-2);
									firsta[lnum]=str1;
								}
								else if(sn[i][sn[i].length()-1]=='H')
								{
									str1+=sn[i].substr(0,sn[i].length()-1);
									firsta[lnum]=str1;
								}
								else
								{
									str1+=sn[i];
									firsta[lnum]=str1;
								}
							}
						}							
						laddr[lnum]=addr;
						lnum++;
						addr+=width;
						datal+=str1;
						return machcode;
						}
						else	{}																						//	不匹配
					}
					else																								//有两个操作数
					{
						if((st[1]==sc[1])&&(st[2]==sc[2])&&(st[3]==sc[3]))					//匹配
						{	firsta[lnum]=str1;
						//建立标号表
						if(sc[0]!="\0")
						{
							lab_addr[lnum]=addr;	
							sa[num]=sc[0];
							sad[num]=addr;
							num++;
						}
						else						{}
						for(int i=2;i<=4;i++)
						{
							if(sc[i].find("arg")!=-1)
							{
								if((sn[i][0]=='#')&&sn[i][sn[i].length()-1]=='H')
								{
									str1+=sn[i].substr(1,sn[i].length()-2);
									firsta[lnum]=str1;
								}
								else if(sn[i][sn[i].length()-1]=='H')
								{
									str1+=sn[i].substr(0,sn[i].length()-1);
									firsta[lnum]=str1;
								}
								else
								{
									str1+=sn[i];
									firsta[lnum]=str1;
								}
							}
						}							
						laddr[lnum]=addr;
						lnum++;
						addr+=width;
						datal+=str1;
						return machcode;
						}
						else		{}													//	不匹配
					}
				}
				else																	//只有一个操作数
				{
					if((st[1]==sc[1])&&st[2]==sc[2])					//匹配
					{firsta[lnum]=str1;
					//建立标号表
					if(sc[0]!="\0")
					{
						lab_addr[lnum]=addr;	
						sa[num]=sc[0];
						sad[num]=addr;
						num++;
					}
					else				{}
					for(int i=2;i<=4;i++)
					{
						if(sc[i].find("arg")!=-1)
						{
							if((sn[i][0]=='#')&&sn[i][sn[i].length()-2]=='H')
							{
								str1+=sn[i].substr(1,sn[i].length()-1);
								firsta[lnum]=str1;
							}
							else if(sn[i][sn[i].length()-1]=='H')
							{
								str1+=sn[i].substr(0,sn[i].length()-1);
								firsta[lnum]=str1;
							}
							else
							{
								str1+=sn[i];
								firsta[lnum]=str1;
							}
						}
					}							
					laddr[lnum]=addr;
					lnum++;
					addr+=width;
					datal+=str1;
					return machcode;
					}
					else		{}															//	不匹配	
				}
			}
			else																		//没有操作数
			{
				if(st[1]==sc[1])													//匹配									
				{					firsta[lnum]=str1;
				//建立标号表
				if(sc[0]!="\0")
				{
					lab_addr[lnum]=addr;	
					sa[num]=sc[0];
					sad[num]=addr;
					num++;
				}
				else				{}
				for(int i=2;i<=4;i++)
				{
					if(sc[i].find("arg")!=-1)
					{
						if((sn[i][0]=='#')&&sn[i][sn[i].length()-2]=='H')
						{
							str1+=sn[i].substr(1,sn[i].length()-1);
							firsta[lnum]=str1;
						}
						else if(sn[i][sn[i].length()-1]=='H')
						{
							str1+=sn[i].substr(0,sn[i].length()-1);
							firsta[lnum]=str1;
						}
						else
						{
							str1+=sn[i];
							firsta[lnum]=str1;
						}
					}
				}							
				laddr[lnum]=addr;
				lnum++;
				addr+=width;
				datal+=str1;
				return machcode;
				}
				else		{}															//不匹配
			}
		}//while结束

		//源文件中没有匹配的指令
		//for(int i=0;i<=4;i++)
		//{
		//	cout<<s[i]<<"\t";
		//}
		//cout<<"这条不匹配"<<endl;
	}//if(in)结束

	else // 没有该文件
	{
		cout << "打开文件" << filename << "出错！" << endl;
		exit(-1);
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
		return op_mne;
	}
	else
	{
		op_mne=s;
		return op_mne;
	}
}

string Inst::get_op_num(string s, int i)
{
	int j;
	j=s.find(",");
	if(j!=-1)																				//不止一个操作数
	{
		op_num[i]=s.substr(0,j);
		return op_num[i];	
	}
	else
	{
		op_num[i]=s;
		return op_num[i];
	}
}

inline string Inst::get_label(string s)									//获取标号，代码少，定义为内联函数
{
	int i;
	i=s.find(":");
	label=s.substr(0,i);
	return label;
}

//将十进制整数转为十六进制字符串，可以设置任意长度，方便地址和机器码两种长度
string Inst::int_to_string(int t, int n)			//t是数据，n是宽度
{
	stringstream ss;
	string str;
	ss<<hex<<t;										//将十进制整数输入后转为十六进制
	ss>>str;
	for(int i=1;i<=n;i++)
	{
		if((str.length()<n)&&(str.length()>0))
			str="0"+str;
		else if(str.length()>n)
		{
			cout<<"整数太大，超出转换宽度！"<<endl;
			exit(-1);
		}
		else	{}
	}
	transform(str.begin(), str.end(), str.begin(), toupper);				//转换为大写
	return str;
}

int Inst::string_to_int(string s)					//十六进制字符串转成十进制整数
{
	const char *t = s.data();
	int i=0, tmp, result=0;
	for(i=0; i<strlen(t); i++)						 // 把字符一个一个转成16进制数 
	{
		if((t[i]>='0')&&(t[i]<='9'))
			tmp = t[i]-'0';
		else if((t[i]>='A')&&(t[i]<='F'))
			tmp = t[i]-'A'+10;
		else if((t[i]>='a')&&(t[i]<='f'))
			tmp = t[i]-'a'+10;
		else
		{
			cout<<s<<"出错了"<<endl;
			exit(-1);
			//return -1;  /* 出错了 */				 //这里不能返回-1！！！
		}
		result= result*16+tmp;						// 转成16进制数后加起来 
	}
	return result;
}

string Inst::match(string dataline)				//拼接字符串
{
	string sen,tmp;										//sen存储一行，tmp临时
	int w;
	w=dataline.length();								//计算数据长度域
	tmp=int_to_string(w,2);							//转换数据长度域为字符串
	sen.append(tmp);									//加入数据长度域
	tmp=int_to_string(addr_s,4);					//将起始地址转换为字符串
	sen.append(tmp);									//加入起始地址
	sen.append("00");									//加入数据类型
	sen.append(dataline);							//加入数据串
	sen.append(get_checksum(dataline));	//加入校验和
	return sen;
}

string Inst::get_checksum(string dataline)		//获得校验和
{
	int n,i=0;
	string s1,s2,str[100],checks,tmps;
	int val[100];
	int j=0,tmp=0, result=0;
	s1=int_to_string(dataline.length(),2);			//表示有效数据字节数的两个字符
	s2=int_to_string(addr_s,4);						//表示起始地址的四个字符
	tmps=s1+s2+"00"+dataline;					//前面8个字符外加数据串
	n=tmps.length();
	for(i=0;i<(n/2);i++)									
	{
		str[i]=tmps.substr(0,2);							//每两个字符截断tmps
		val[i]=string_to_int(str[i]);						//转换字符串为整数并存入数组
		tmps=tmps.substr(2,tmps.length());
	}

	//计算校验和
	for(i=0;i<(n/2);i++)									//累加拆开的数组
		tmp=tmp+val[i];
	tmp=~tmp+1;
	stringstream ss;
	ss<<hex<<tmp;										//将十进制整数输入后转为十六进制
	ss>>checks;
	transform(checks.begin(), checks.end(), checks.begin(), toupper);		//转小写为大写
	checks=checks.substr(checks.length()-2,checks.length());					//截取最后两位字符
	return checks;
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
	string str,s[5],tmp[5];								//str储存读取的每行字符串，s[]保存截断的每部分
	int i;
	while(std::getline(fin,str))
	{
		if(str.find(":")!=-1)								//如果有标号
		{
			s[0]=inst.get_label(str);					//获取标号
			str=str.substr(str.find(":")+1,str.length());			//截取":"后的字符串
		}
		else
		{
			s[0]="\0";
		}
		s[1]=inst.get_mne(str);											//先获取助记符
		if(str.find(" ")!=-1)													//如果有操作数
		{
			str=str.substr(str.find(" ")+1,str.length());			//截取" "后的字符串
			if(str.find(",")!=-1)												//如果有不止一个操作数
			{
				s[2]=inst.get_op_num(str,0);							//获取第一操作数
				str=str.substr(str.find(",")+1,str.length());		//截取","后的字符串
				if(str.find(",")!=-1)											//如果有三个操作数
				{
					s[3]=inst.get_op_num(str,1);						//获取第二操作数
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
				//将地址转换为arg，但是保留原数组，方便匹配HEX.txt内的每一行指令
				if((sn[i]=="A")||(sn[i]=="B")||(sn[i]=="C")||(sn[i]=="AB")||(sn[i]=="DPTR")||(sn[i][0]=='R')||(sn[i]=="A+DPTR"))		{}
				else if((sn[i][0]=='@')||(sn[i][0]=='/'))			{}
				else if((sn[1]=="ORG")||(sn[1]=="END"))			{}
				else
				{
					if(sn[i][0]=='#')		sn[i]="#arg";
					else if(sn[i][sn[i].length()-1]=='H')	sn[i]="arg";
					else	sn[i]="arg";
				}
			}
			else		{}
		}
		for(i=0;i<=4;i++)
		{
			tmp[i]=sn[i];
			sn[i]=s[i];
			s[i]=tmp[i];
		}
		inst.get_machcode(s);
	}//while结束
}


void get_obj(string se)									//输出hex文档
{
	ofstream in;
	Inst inst;
	int l;
	string str;
	l=se.length();
	inst.get_checksum(datal);
	in.open("obj.txt",ios::app);							//ios::trunc表示在打开文件前将文件清空，app是接上，由于是写入,文件不存在则创建
	in<<":"<<inst.match(se)<<"\n";
	in.close();													//关闭文件
}

string replacelab(string dataline, const string oldc, const string newc)									//替换字符串
{
	string::size_type   pos(0);   
	if(   (pos=dataline.find(oldc,pos))!=string::npos   )   
		dataline.replace(pos,oldc.length(),newc);   
	else	{}
	return dataline;   
} 

FILE *fp_in;
FILE *fp_out;

//const char *fake[]={	"ORG","DB","DW","EQU","DATA","XDATA","BIT","END"	};			//伪指令

int main()
{
	Inst inst;
	ofstream in;
	stringstream ss;
	readasm(inst);
	string ta;
	int i=0,j=0,t;
	for(i=0;i<num;i++)
	{	
		int n=0;
		for(j=0;j<lnum;j++)
		{
			if(firsta[j].find(sa[i])!=-1)
			{
				t=~(laddr[j]-sad[i])-n-1;
				ta=inst.int_to_string(t,16);										//将十进制整数输入后转为十六进制
				ta=ta.substr(ta.length()-2,ta.length());
				datal=replacelab(datal,sa[i],ta);
				n++;
			}
			else			{}
		}
	}
	string str;
	while(datal.length()>16)													//如果数据串太长，超过16字节就分割
	{
		str=datal.substr(0,16);
		datal=datal.substr(16,datal.length());
		get_obj(str);
		addr_s+=16;
	}
	str=datal;
	get_obj(str);
	in.open("obj.txt",ios::app);
	in<<":00000001FF"<<"\n";												//添加结束语句
	return 1;
}
