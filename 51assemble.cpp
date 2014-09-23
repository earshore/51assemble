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
	int machcode;				//������
	int width;						//�ֽ���
	string label;					//���
	string op_mne;				//���Ƿ�
	string op_num[3];			//������
public:
	Inst()								//���캯����ʼ��
	{
		machcode=-1;
		width=0;
		label="\0";
		op_mne="\0";
		string op_num[3]={"\0","\0","\0"};
	}
	~Inst()	{}
	int get_machcode(string *s);							//��ȡ������
	string get_mne(string s);								//��ȡ���Ƿ�
	string get_op_num(string s, int i);					//��ȡ������
	inline string get_label(string s);						//��ȡ���
	friend void get_obj(string se);						//���hex�ļ�
	string match(string machc);							//����ַ���
	string int_to_string(int t,int n);						//����ת�����ַ���
	int string_to_int(string s);								//�ַ���ת����
	string get_checksum(string machc);				//���У����
};

static int addr=0;						//��ַ
static int addr_s=0;					//��ʼ��ַ������������ĵ�ʱ
static int lnum=0;						//ָ������ 
static int laddr[100];					//ÿ��ָ�����ڵ�ַ
static string firsta[100];				//��һ�λ��õ��ַ���
static string mc="\0";				//�������ַ���
static string sn[5];						//��¼Դ�ļ�ÿ��ָ���ÿ������
static int num=0;						//��¼��ű����
static string sa[100];					//��ű�洢���
static int sad[100];					//��ű����ַ

static int lab_addr[100];			//���ڽ�����ű�ʱ�����Ŵ���ĵ�ַ
void readasm(Inst inst);				//��ȡԴ����

string replacelab(string machc, const string oldc, const string newc);		//�滻��һ��Ŀ���ַ���

int Inst::get_machcode(string *s)											//��ȡָ���Ӧ�Ļ�����
{
	ifstream in("HEX.txt");
	string filename;
	string str,str1,str2,str3,st[5],sc[5];
	//str�洢��ȡ��HEX.txt��һ������ָ��������ƻ�
	//str1��������벿��
	//str2�����ֽڿ��
	//st[5]���������
	//sc[5]����s[5]����ֹs[5]�ƻ�
	//la[2]�洢αָ������Ӧ�ĵ�ַ
	for(int n=0;n<5;n++)
		sc[n]=s[n];
	st[0]="\0";
	if(in) // �и��ļ�
	{
		while (getline (in, str))													// str�в�����ÿ�еĻ��з�
		{
			//�����ж�αָ��
			if(sc[1]=="ORG")														//����ǣ�����αָ����ַ
			{
				sc[2]=s[2].substr(0,s[2].length()-1);			
				addr_s=string_to_int(sc[2]);
				addr=addr_s;
				laddr[lnum]=addr;
				//cout<<addr<<endl;
			}
			else if(sc[1]=="END")
			{}
			else
			{}
			//cout<<hex<<addr<<endl;
			str2=str.substr(str.find(":")+1,1);								//�ض϶�ȡ����һ���ַ������ֽڳ��Ȳ���
			str1=str.substr(0,2);													//�ض϶�ȡ����һ���ַ����л����벿��
			str3=str.substr(str.find(":")+1,str.length());					//���ಿ��
			str3=str3.substr(str3.find(":")+1,str3.length());

			//��str1��str2������������ֽڳ�����ʮ�������ַ�������ת��Ϊʮ��������
			int result1=0,result2=0;
			result1=string_to_int(str1);
			result2=string_to_int(str2);
			machcode=result1;																				//������
			width=result2;																						//�ֽڳ���
			//cout<<str1<<"\t"<<result1<<"\t"<<result2<<endl;						//����result

			//ƥ��ָ����ػ�����
			st[1]=str3.substr(0,str3.find(":"));
			str3=str3.substr(str3.find(":")+1,str3.length());
			if(sc[2]!="\0")																						//������һ��������
			{
				st[2]=str3.substr(0,str3.find(":"));
				str3=str3.substr(str3.find(":")+1,str3.length());
				if(sc[3]!="\0")																					//����������������
				{
					st[3]=str3.substr(0,str3.find(":"));
					str3=str3.substr(str3.find(":")+1,str3.length());
					if(sc[4]!="\0")																				//������������
					{
						st[4]=str3.substr(0,str3.find(":"));
						str3=str3.substr(str3.find(":")+1,str3.length());
						if((st[1]==sc[1])&&(st[2]==sc[2])&&(st[3]==sc[3])&&(st[4]==sc[4]))		//ƥ��
						{firsta[lnum]=str1;
						//������ű�
						if(sc[0]!="\0")
						{
							lab_addr[lnum]=addr;
							sa[num]=sc[0];
							sad[num]=addr;
							num++;
						}
						else
						{}
						for(int i=2;i<=4;i++)
						{
							if(sc[i].find("arg")!=-1)
							{
								if((sn[i][0]=='#')&&(sn[i][sn[i].length()-1]=='H'))
								{
									str1+=sn[i].substr(1,sn[i].length()-2);
									firsta[lnum]=str1;
									//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
								}
								else if(sn[i][sn[i].length()-1]=='H')
								{
									str1+=sn[i].substr(0,sn[i].length()-1);
									firsta[lnum]=str1;
									//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
								}
								else
								{
									str1+=sn[i];
									firsta[lnum]=str1;
									//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
								}
								//		cout<<str1<<endl;
							}

						}							
						laddr[lnum]=addr;
						lnum++;
						//cout<<laddr[lnum]<<endl;
						addr+=width;
						mc+=str1;
						//	cout<<mc<<endl;															//��������봮
						return machcode;
						}
						else																							//	��ƥ��
						{}
					}
					else																								//������������
					{
						if((st[1]==sc[1])&&(st[2]==sc[2])&&(st[3]==sc[3]))					//ƥ��
						{	firsta[lnum]=str1;
						//������ű�
						if(sc[0]!="\0")
						{
							lab_addr[lnum]=addr;	
							sa[num]=sc[0];
							sad[num]=addr;
							num++;
						}
						else
						{}
						//cout<<str1<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;	//���ƥ���ָ��Ļ�������ֽڳ����Լ���ǰ��ַ
						//cout<<sc[0]<<endl;
						for(int i=2;i<=4;i++)
						{
							if(sc[i].find("arg")!=-1)
							{
								if((sn[i][0]=='#')&&sn[i][sn[i].length()-1]=='H')
								{
									str1+=sn[i].substr(1,sn[i].length()-2);
									firsta[lnum]=str1;
									//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
								}
								else if(sn[i][sn[i].length()-1]=='H')
								{
									str1+=sn[i].substr(0,sn[i].length()-1);
									firsta[lnum]=str1;
									//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
								}
								else
								{
									str1+=sn[i];
									firsta[lnum]=str1;
									//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
								}
								//	cout<<str1<<endl;
							}

						}							
						laddr[lnum]=addr;
						lnum++;
						//	cout<<laddr[lnum]<<endl;
						addr+=width;
						mc+=str1;
						//	cout<<mc<<endl;							//��������봮
						return machcode;
						}
						else															//	��ƥ��
						{}
					}
				}
				else																	//ֻ��һ��������
				{
					if((st[1]==sc[1])&&st[2]==sc[2])					//ƥ��
					{firsta[lnum]=str1;
					//������ű�
					if(sc[0]!="\0")
					{
						lab_addr[lnum]=addr;	
						sa[num]=sc[0];
						sad[num]=addr;
						num++;
					}
					else
					{}

					for(int i=2;i<=4;i++)
					{
						if(sc[i].find("arg")!=-1)
						{
							if((sn[i][0]=='#')&&sn[i][sn[i].length()-2]=='H')
							{
								str1+=sn[i].substr(1,sn[i].length()-1);
								firsta[lnum]=str1;
								//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
							}
							else if(sn[i][sn[i].length()-1]=='H')
							{
								str1+=sn[i].substr(0,sn[i].length()-1);
								firsta[lnum]=str1;
								//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
							}
							else
							{
								str1+=sn[i];
								firsta[lnum]=str1;
								//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
							}
							//	cout<<str1<<endl;
						}

					}							
					laddr[lnum]=addr;
					lnum++;
					//	cout<<laddr[lnum]<<endl;
					addr+=width;
					mc+=str1;
					//	cout<<mc<<endl;									//��������봮
					return machcode;
					}
					else																//	��ƥ��
					{}		
				}
			}
			else																		//û�в�����
			{
				if(st[1]==sc[1])													//ƥ��									
				{					firsta[lnum]=str1;
				//������ű�
				if(sc[0]!="\0")
				{
					lab_addr[lnum]=addr;	
					sa[num]=sc[0];
					sad[num]=addr;
					num++;
				}
				else
				{}
				for(int i=2;i<=4;i++)
				{
					if(sc[i].find("arg")!=-1)
					{
						if((sn[i][0]=='#')&&sn[i][sn[i].length()-2]=='H')
						{
							str1+=sn[i].substr(1,sn[i].length()-1);
							firsta[lnum]=str1;
							//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
						}
						else if(sn[i][sn[i].length()-1]=='H')
						{
							str1+=sn[i].substr(0,sn[i].length()-1);
							firsta[lnum]=str1;
							//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
						}
						else
						{
							str1+=sn[i];
							firsta[lnum]=str1;
							//cout<<sc[i]<<"\t"<<sn[i]<<"\t";
						}
						//cout<<str1<<endl;
					}

				}							
				laddr[lnum]=addr;
				lnum++;
				//		cout<<laddr[lnum]<<endl;
				addr+=width;
				mc+=str1;
				//	cout<<mc<<endl;										//��������봮
				return machcode;
				}
				else																	//��ƥ��
				{}
			}
		}//while����

		/*	for(int i=0;i<num;i++)
		cout<<sa[i]<<"\t"<<sad[i]<<endl;*/

		//Դ�ļ���û��ƥ���ָ��
		//for(int i=0;i<=4;i++)
		//{
		//	cout<<s[i]<<"\t";
		//}
		//cout<<"������ƥ��"<<endl;
	}
	else // û�и��ļ�
	{
		cout << "���ļ�" << filename << "����" << endl;
		exit(-1);
	}
}//get_machcode����

string Inst::get_mne(string s)
{
	//��ȡ���Ƿ�
	int i;
	i=s.find(" ");
	if(i!=-1)
	{
		op_mne=s.substr(0,i);
		//cout<<"���Ƿ�:"<<op_mne<<endl;							//������Ƿ�
		return op_mne;
	}
	else
	{
		op_mne=s;
		//cout<<"���Ƿ�:"<<op_mne<<endl;							//������Ƿ�
		return op_mne;
	}
}

string Inst::get_op_num(string s, int i)
{
	int j;
	j=s.find(",");
	if(j!=-1)																				//��ֹһ��������
	{
		op_num[i]=s.substr(0,j);
		//cout<<"������"<<i+1<<":"<<op_num[i]<<endl;		//���������
		return op_num[i];	
	}
	else
	{
		//op_num[i]=s.substr(s.find(" "),s.length());
		op_num[i]=s;
		//cout<<"������"<<i+1<<":"<<op_num[i]<<endl;		//���������
		return op_num[i];
	}
}

inline string Inst::get_label(string s)
{
	int i;
	i=s.find(":");
	label=s.substr(0,i);
	//	cout<<"���:"<<label<<endl;
	return label;
}

//��ʮ��������תΪʮ�������ַ����������������ⳤ�ȣ������ַ�ͻ��������ֳ���
string Inst::int_to_string(int t, int n)			//t�����ݣ�n�ǿ��
{
	stringstream ss;
	string str;
	ss<<hex<<t;										//��ʮ�������������תΪʮ������
	ss>>str;
	for(int i=1;i<=n;i++)
	{
		if((str.length()<n)&&(str.length()>0))
		{
			str="0"+str;
		}
		else if(str.length()>n)
		{
			cout<<"����̫�󣬳���ת����ȣ�"<<endl;
			exit(-1);
		}
		else
		{}
	}
	transform(str.begin(), str.end(), str.begin(), toupper);				//ת��Ϊ��д
	//	cout<<str<<endl;							//У�����
	return str;
}

int Inst::string_to_int(string s)					//ʮ�������ַ���ת��ʮ��������
{
	const char *t = s.data();
	//cout<<s<<endl;								//У������
	int i=0, tmp, result=0;
	for(i=0; i<strlen(t); i++)						 // ���ַ�һ��һ��ת��16������ 
	{
		if((t[i]>='0')&&(t[i]<='9'))
			tmp = t[i]-'0';
		else if((t[i]>='A')&&(t[i]<='F'))
			tmp = t[i]-'A'+10;
		else if((t[i]>='a')&&(t[i]<='f'))
			tmp = t[i]-'a'+10;
		else
		{
			cout<<s<<"������"<<endl;
			exit(-1);
			//return -1;  /* ������ */				 //���ﲻ�ܷ���-1������
		}
		//cout<<t[i]<<endl;							//����t[i]
		result= result*16+tmp;						// ת��16������������� 
	}
	//cout<<s<<"\t"<<result<<endl;		//У����
	return result;
}

string Inst::match(string machc)				//ƴ���ַ���
{
	string sen,tmp;										//sen�洢һ�У�tmp��ʱ
	int w;
	w=machc.length();								//�������ݳ�����
	tmp=int_to_string(w,2);							//ת�����ݳ�����Ϊ�ַ���
	sen.append(tmp);									//�������ݳ�����
	tmp=int_to_string(addr_s,4);					//����ʼ��ַת��Ϊ�ַ���
	sen.append(tmp);									//������ʼ��ַ
	sen.append("00");									//������������
	sen.append(machc);								//���������
	sen.append(get_checksum(machc));
	//cout<<sen<<endl;								//����ǰ��8���ַ�
	return sen;
}

string Inst::get_checksum(string machc)		//���У��ͣ�s�ǻ�����
{
	int n,i=0;
	string s1,s2,str[100],checks,tmps;
	int val[100];
	int j=0,tmp=0, result=0;
	s1=int_to_string(machc.length(),2);			//��ʾ��Ч�����ֽ����������ַ�
	s2=int_to_string(addr_s,4);						//��ʾ��ʼ��ַ���ĸ��ַ�
	tmps=s1+s2+"00"+machc;						//ǰ��8���ַ���ӻ�����
	n=tmps.length();

	//cout<<tmps.substr(0,2)<<"\t"<<s1<<endl;									//����s1
	//cout<<s2<<"\t"<<addr_s<<"\t"<<hex<<addr_s<<endl;			//������ʵ��ַ
	//cout<<s<<endl;																			//��������봮
	//cout<<tmps<<"\t"<<tmps.length()<<endl;									//�������ַ������˴�tmpsӦ����һ���ַ����Լ��䳤��

	for(i=0;i<(n/2);i++)									
	{
		str[i]=tmps.substr(0,2);							//ÿ�����ַ��ض�tmps
		val[i]=string_to_int(str[i]);						//ת���ַ���Ϊ��������������
		//cout<<tmps.substr(0,2)<<" ";			//����ض����
		tmps=tmps.substr(2,tmps.length());
		//cout<<str[i]<<"\t"<<val[i]<<endl;	//����ת�����
	}
	//cout<<tmps<<endl;								//�������ַ������˴�tmpsӦ��Ϊ��

	//����У���
	for(i=0;i<(n/2);i++)									//�ۼӲ𿪵�����
		tmp=tmp+val[i];
	//cout<<tmp<<endl;
	tmp=~tmp+1;
	//cout<<tmp<<endl;
	stringstream ss;
	ss<<hex<<tmp;										//��ʮ�������������תΪʮ������
	ss>>checks;
	transform(checks.begin(), checks.end(), checks.begin(), toupper);		//תСдΪ��д
	//cout<<tmp<<"\t"<<checks.substr(checks.length()-2,checks.length())<<endl;
	checks=checks.substr(checks.length()-2,checks.length());					//��ȡ�����λ�ַ�
	return checks;
}

void readasm(Inst inst)
{
	string filename = "data.txt";					//Դ�ļ�
	ifstream fin(filename.c_str());  
	if( !fin )													//����
	{   
		cout << "���ļ�" << filename << "����" << endl;   
		exit(-1);
	}
	string str,s[5],tmp[5];								//str�����ȡ��ÿ���ַ�����s[]����ضϵ�ÿ����
	int i;
	while(std::getline(fin,str))
	{
		if(str.find(":")!=-1)								//����б��
		{
			s[0]=inst.get_label(str);					//��ȡ���
			str=str.substr(str.find(":")+1,str.length());			//��ȡ":"����ַ���
		}
		else
		{
			s[0]="\0";
		}
		s[1]=inst.get_mne(str);											//�Ȼ�ȡ���Ƿ�
		if(str.find(" ")!=-1)													//����в�����
		{
			str=str.substr(str.find(" ")+1,str.length());			//��ȡ" "����ַ���
			if(str.find(",")!=-1)												//����в�ֹһ��������
			{
				s[2]=inst.get_op_num(str,0);							//��ȡ��һ������
				str=str.substr(str.find(",")+1,str.length());		//��ȡ","����ַ���
				if(str.find(",")!=-1)											//���������������
				{
					s[3]=inst.get_op_num(str,1);						//��ȡ�ڶ�������
					str=str.substr(str.find(",")+1,str.length());	//��ȡ","����ַ���
					s[4]=inst.get_op_num(str,2);						//��ȡ����������
				}
				else
				{
					s[4]="\0";
					s[3]=inst.get_op_num(str,1);						//��ȡ�ڶ�������
				}
			}
			else
			{
				s[3]="\0";
				s[4]="\0";
				s[2]=inst.get_op_num(str,0);							//��ȡ��һ������
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
				//����ַת��Ϊarg�����Ǳ���ԭ���飬����ƥ��HEX.txt�ڵ�ÿһ��ָ��
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

		//for(i=0;i<=4;i++)							//����sn[]
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

		//for(i=0;i<=4;i++)							//����s[]
		//{
		//	cout<<sn[i]<<"\t";
		//}
		//cout<<endl;
		inst.get_machcode(s);
	}
}


void get_obj(string se)									//���hex�ĵ�
{
	ofstream in;
	Inst inst;
	int l;
	string str;
	l=se.length();
	inst.get_checksum(mc);
	in.open("obj.txt",ios::app);							//ios::trunc��ʾ�ڴ��ļ�ǰ���ļ���գ�app�ǽ��ϣ�������д��,�ļ��������򴴽�
	in<<":"<<inst.match(se)<<"\n";
	//in<<":"<<setw(2) <<setfill('0')<<setiosflags(ios::uppercase)<<hex<<machcode<<dec<<addr<<endl;
	in.close();													//�ر��ļ�
}

string replacelab(string machc, const string oldc, const string newc)									//�滻�ַ���
{
	string::size_type   pos(0);   
	if(   (pos=machc.find(oldc,pos))!=string::npos   )   
		machc.replace(pos,oldc.length(),newc);   
	else
	{}
	return machc;   
} 

FILE *fp_in;
FILE *fp_out;

//const char *fake[]={	"ORG","DB","DW","EQU","DATA","XDATA","BIT","END"	};			//αָ��

int main()
{
	Inst inst;
	ofstream in;
	stringstream ss;
	readasm(inst);
	string ta;
	int i=0,j=0,t;
	//cout<<mc<<endl;
	for(i=0;i<num;i++)
	{	
		int n=0;
		for(j=0;j<lnum;j++)
		{
			if(firsta[j].find(sa[i])!=-1)
			{
				t=~(laddr[j]-sad[i])-n-1;
				//cout<<laddr[j]<<"\t"<<sad[i]<<endl;
				ta=inst.int_to_string(t,16);										//��ʮ�������������תΪʮ������
				//cout<<sa[i]<<"\t"<<t<<"\t"<<ta<<endl;
				ta=ta.substr(ta.length()-2,ta.length());
				//cout<<sa[i]<<"\t"<<t<<"\t"<<ta<<endl;
				mc=replacelab(mc,sa[i],ta);
				//cout<<i<<"\t"<<j<<"\t"<<sa[i]<<endl;
				n++;
			}
			else
			{}
		}
	}
	//cout<<mc<<endl;
	string str;
	//	inst.int_to_string(256,5);			//����int_to_string
	//inst.string_to_int("FFF");				//����string_to_int
	//inst.get_checksum(mc);				//����get_checksum

	while(mc.length()>16)					//��������봮̫��������16�ֽھͷָ�
	{
		str=mc.substr(0,16);
		//	cout<<hex<<addr_s<<endl;
		mc=mc.substr(16,mc.length());
		get_obj(str);
		addr_s+=16;
	}
	str=mc;
	//	cout<<hex<<addr_s<<endl;
	get_obj(str);
	//addr_s+=mc.length();
	in.open("obj.txt",ios::app);
	in<<":00000001FF"<<"\n";			//��ӽ������
	//for(int i=0;i<num;i++)
	//cout<<lab[i]<<"\t"<<lab_add[i]<<endl;
	return 1;
}
