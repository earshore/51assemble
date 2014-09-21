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
	int machcode;		//������
	int width;			//�ֽ���
	string label;		//���
	string op_mne;		//���Ƿ�
	string op_num[3];	//������

public:
	Inst()				//���캯����ʼ��
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
	int get_machcode(string *s);				//��ȡ������
	string get_mne(string s);				//��ȡ���Ƿ�
	string get_op_num(string s, int i);		//��ȡ������
	string get_label(string s);				//��ȡ���
	int get_addr(string s);				//��ȡ��ǰָ���ַ
	friend void get_obj(string se);		//���hex�ļ�
	string match(string machc);								//����ַ���
	string int_to_string(int t,int n);				//����ת�����ַ���
	int string_to_int(string s);		//�ַ���ת����
	string get_checksum(string machc);					//���У����
};
static int addr=0;				//��ַ
static int addr_s=0;			//��ʼ��ַ
static string mc;				//�������ַ���

void readasm(Inst inst);						//��ȡԴ����


int Inst::get_machcode(string *s)
{
	//��ȡ����ָ���Ӧ�Ļ�����
	ifstream in("HEX.txt");
	string filename;
	string str,str1,str2,str3,st[5],sc[5],la[2];
	//str�Ƕ�ȡ��HEX.txt��һ������ָ��������ƻ�
	//str1��������벿��
	//str2�����ֽڿ��
	//st[5]���������
	//sc[5]����s[5]����ֹs[5]�ƻ�
	//la[2]�洢αָ������Ӧ�ĵ�ַ
	//stringstream ss;
	for(int n=0;n<5;n++)
		sc[n]=s[n];
	st[0]="\0";

	if(in) // �и��ļ�
	{
		while (getline (in, str)) // str�в�����ÿ�еĻ��з�
		{
			//�����ж�αָ��
			if(sc[1]=="ORG")												//����ǣ�����αָ����ַ
			{
				sc[2]=s[2].substr(0,s[2].length()-1);			
				addr_s=atoi(sc[2].c_str());
				//cout<<addr_s<<endl;
				addr=atoi(sc[2].c_str());
				la[0]=sc[1];
				la[1]=sc[2];

			}
			else if(sc[1]=="END")
			{
				la[0]=sc[1];
				la[1]=int_to_string(addr,4);					//������ת�ַ���

			}
			else
			{

				str2=str.substr(str.find(":")+1,1);								//�ض϶�ȡ����һ���ַ������ֽڳ��Ȳ���
				str1=str.substr(0,2);													//�ض϶�ȡ����һ���ַ����л����벿��
				str3=str.substr(str.find(":")+1,str.length());					//���ಿ��
				//	cout<<str<<endl;												//����
				str3=str3.substr(str3.find(":")+1,str3.length());
				//cout<<str<<endl;													//����

				//��str1��str2������������ֽڳ������ַ�������ת��Ϊʮ��������
				const char *t = str1.data();
				const char *w=str2.data();
				int i=0, tmp, result1=0,result2=0;
				for(i=0; i<strlen(t); i++) /* ���ַ�һ��һ��ת��16������ */
				{
					if((t[i]>='0')&&(t[i]<='9'))
						tmp = t[i]-'0';
					else if((t[i]>='A')&&(t[i]<='F'))
						tmp = t[i]-'A'+10;
					else if((t[i]>='a')&&(t[i]<='f'))
						tmp = t[i]-'a'+10;
					else
					{
						cout<<"������"<<endl;
						exit(-1);
					}
					result1 = result1*16+tmp;  /* ת��16������������� */
				}
				tmp=w[0]-'0';
				result2 = result2*16+tmp;  /* ת��16������������� */
				machcode=result1;							//������
				width=result2;									//�ֽڳ���
				//cout<<t<<"\t"<<result1<<"\t"<<result2<<endl;							//����result

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
							{

								//������ű�
								if(sc[0]!="\0")
								{
									la[0]=sc[0];
									la[1]=int_to_string(addr,4);			//������ת�ַ���
									//cout<<la[0]<<"\t"<<la[1]<<endl;
								}
								else
								{}

								//	cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;		//���ƥ���ָ��Ļ�������ֽڳ����Լ���ǰ��ַ
								//cout<<sc[0]<<endl;
								addr+=width;
								mc+=str1;
								//cout<<mc<<endl;				//��������봮
								return machcode;
							}
							else													//	��ƥ��
							{}
						}
						else																							//������������
						{
							if((st[1]==sc[1])&&(st[2]==sc[2])&&(st[3]==sc[3]))							//ƥ��
							{	
								//������ű�
								if(sc[0]!="\0")
								{
									la[0]=sc[0];
									la[1]=int_to_string(addr,4);					//������ת�ַ���

									//		cout<<la[0]<<"\t"<<la[1]<<endl;
								}
								else
								{}

								//		cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;	//���ƥ���ָ��Ļ�������ֽڳ����Լ���ǰ��ַ
								//cout<<sc[0]<<endl;
								addr+=width;
								mc+=str1;
								//cout<<mc<<endl;				//��������봮
								return machcode;
							}
							else													//	��ƥ��
							{}
						}
					}
					else																							//ֻ��һ��������
					{
						if((st[1]==sc[1])&&st[2]==sc[2])				//ƥ��
						{

							//������ű�
							if(sc[0]!="\0")
							{
								la[0]=sc[0];
								la[1]=int_to_string(addr,4);					//������ת�ַ���

								//		cout<<la[0]<<"\t"<<la[1]<<endl;
							}
							else
							{}

							//	cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;	//���ƥ���ָ��Ļ�������ֽڳ����Լ���ǰ��ַ
							//	cout<<sc[0]<<endl;
							addr+=width;
							mc+=str1;
							//cout<<mc<<endl;				//��������봮
							return machcode;
						}
						else													//	��ƥ��
						{}
					}
				}
				else																								//û�в�����
				{
					if(st[1]==sc[1])										//ƥ��									
					{					

						//������ű�
						if(sc[0]!="\0")
						{
							la[0]=sc[0];
							la[1]=int_to_string(addr,4);					//������ת�ַ���

							//		cout<<la[0]<<"\t"<<la[1]<<endl;
						}
						else
						{}

						//		cout<<t<<"\t"<<result1<<"\t"<<result2<<"\t"<<addr<<endl;	//���ƥ���ָ��Ļ�������ֽڳ����Լ���ǰ��ַ
						//	cout<<sc[0]<<endl;
						addr+=width;
						mc+=str1;
						//cout<<mc<<endl;				//��������봮
						return machcode;
					}
					else														//��ƥ��
					{}
				}

			}
		}

		//cout<<la[0]<<"\t"<<la[1]<<endl;				//���αָ���

		//Դ�ļ���û��ƥ���ָ��
		//for(int i=0;i<=4;i++)
		//{
		//	cout<<s[i]<<"\t";
		//}
		//cout<<"������ƥ��"<<endl;

	}//while����

	else // û�и��ļ�
	{
		cout << "���ļ�" << filename << "����" << endl;
		return -1;
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
	if(j!=-1)
	{
		op_num[i]=s.substr(0,j);
		//cout<<"������"<<i+1<<":"<<op_num[i]<<endl;					//���������
		return op_num[i];	
	}
	else
	{
		//op_num[i]=s.substr(s.find(" "),s.length());
		op_num[i]=s;
		//cout<<"������"<<i+1<<":"<<op_num[i]<<endl;					//���������
		return op_num[i];
	}
}

string Inst::get_label(string s)
{
	int i;
	i=s.find(":");
	label=s.substr(0,i);
	//	cout<<"���:"<<label<<endl;
	return label;

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
	string str,s[5],sn[5],tmp[5];						//str�����ȡ��ÿ���ַ�����s[]����ضϵ�ÿ����
	int i;
	while(std::getline(fin,str))
	{

		if(str.find(":")!=-1)								//����б��
		{
			s[0]=inst.get_label(str);							//��ȡ���
			str=str.substr(str.find(":")+1,str.length());	//��ȡ":"����ַ���
		}
		else
		{
			s[0]="\0";
		}
		s[1]=inst.get_mne(str);									//�Ȼ�ȡ���Ƿ�
		if(str.find(" ")!=-1)											//����в�����
		{
			str=str.substr(str.find(" ")+1,str.length());	//��ȡ" "����ַ���
			if(str.find(",")!=-1)										//����в�ֹһ��������
			{
				s[2]=inst.get_op_num(str,0);						//��ȡ��һ������
				str=str.substr(str.find(",")+1,str.length());	//��ȡ","����ַ���
				if(str.find(",")!=-1)										//���������������
				{
					s[3]=inst.get_op_num(str,1);					//��ȡ�ڶ�������
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
				//����ַת��Ϊarg������ƥ��HEX.txt�ڵ�ÿһ��ָ��
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

		//for(i=0;i<=4;i++)						//����sn[]
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
		//for(i=0;i<=4;i++)					//����s[]
		//{
		//	cout<<s[i]<<"\t";
		//}
		//cout<<endl;

		inst.get_machcode(s);
	}

}

//��ʮ��������תΪʮ�������ַ����������������ⳤ�ȣ������ַ�ͻ��������ֳ���
string Inst::int_to_string(int t, int n)					//t�����ݣ�n�ǿ��
{
	stringstream ss;
	string str;
	ss<<hex<<t;							//��ʮ�������������תΪʮ������
	ss>>str;
	for(int i=1;i<=n;i++)
	{
		if(str.length()<n)
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
	//	cout<<str<<endl;				//У�����
	return str;
}

int Inst::string_to_int(string s)
{
	const char *t = s.data();
	//cout<<s<<endl;			//У������
	int i=0, tmp, result=0;
	for(i=0; i<strlen(t); i++) /* ���ַ�һ��һ��ת��16������ */
	{
		if((t[i]>='0')&&(t[i]<='9'))
			tmp = t[i]-'0';
		else if((t[i]>='A')&&(t[i]<='F'))
			tmp = t[i]-'A'+10;
		else if((t[i]>='a')&&(t[i]<='f'))
			tmp = t[i]-'a'+10;
		else
		{
			cout<<"������"<<endl;
			exit(-1);
			//return -1;  /* ������ */       //���ﲻ�ܷ���-1������
		}
		//cout<<t[i]<<endl;				//����t[i]
		result= result*16+tmp;  /* ת��16������������� */
	}
	//cout<<s<<"\t"<<result<<endl;						//У����
	return result;
}

string Inst::match(string machc)				//ƴ���ַ���
{
	string sen,tmp;							//sen�洢һ�У�tmp��ʱ
	int w;
	w=machc.length();					//����������ֽڳ���
	tmp=int_to_string(w,2);				//ת������Ϊ�ַ���
	sen.append(tmp);						//�����ֽڳ���
	//cout<<addr_s<<endl;
	//cout<<addr<<endl;
	tmp=int_to_string(addr_s,4);		//����ʼ��ַת��Ϊ�ַ���
	sen.append(tmp);						//������ʼ��ַ
	sen.append("00");						//������������
	sen.append(machc);						//���������
	sen.append(get_checksum(machc));
	//cout<<sen<<endl;								//����ǰ��8���ַ�
	return sen;
}

string Inst::get_checksum(string machc)			//���У��ͣ�s�ǻ�����
{
	int n,i=0;
	string s1,s2,str[100],checks,tmps;
	int val[100];
	int j=0,tmp=0, result=0;
	s1=int_to_string(machc.length(),2);						//��ʾ��Ч�����ֽ����������ַ�
	s2=int_to_string(addr_s,4);							//��ʾ��ʼ��ַ���ĸ��ַ�
	tmps=s1+s2+"00"+machc;										//ǰ��8���ַ���ӻ�����
	n=tmps.length();

	//cout<<tmps.substr(0,2)<<"\t"<<s1<<endl;	//����s1
	//cout<<s2<<"\t"<<addr_s<<"\t"<<hex<<addr_s<<endl;			//������ʵ��ַ
	//cout<<s<<endl;					//��������봮
	//cout<<tmps<<"\t"<<tmps.length()<<endl;					//�������ַ������˴�tmpsӦ����һ���ַ����Լ��䳤��

	for(i=0;i<(n/2);i++)									
	{
		str[i]=tmps.substr(0,2);							//ÿ�����ַ��ض�tmps
		val[i]=string_to_int(str[i]);						//ת���ַ���Ϊ��������������
		//cout<<tmps.substr(0,2)<<" ";				//����ض����
		tmps=tmps.substr(2,tmps.length());
		//cout<<str[i]<<"\t"<<val[i]<<endl;		//����ת�����
	}
	//cout<<tmps<<endl;					//�������ַ������˴�tmpsӦ��Ϊ��

	//����У���
	for(i=0;i<(n/2);i++)					//�ۼӲ𿪵�����
		tmp=tmp+val[i];
	//cout<<tmp<<endl;
	tmp=~tmp+1;
	//cout<<tmp<<endl;
	stringstream ss;
	ss<<hex<<tmp;							//��ʮ�������������תΪʮ������
	ss>>checks;
	transform(checks.begin(), checks.end(), checks.begin(), toupper);		//תСдΪ��д
	//cout<<tmp<<"\t"<<checks.substr(checks.length()-2,checks.length())<<endl;
	checks=checks.substr(checks.length()-2,checks.length());					//��ȡ�����λ�ַ�
	return checks;
}


void get_obj(string se)		//���hex�ĵ�
{
	ofstream in;
	Inst inst;
	int l;
	string str;
	l=se.length();
	inst.get_checksum(mc);
	in.open("obj.txt",ios::app); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ���գ�app�ǽ��ϣ�������д��,�ļ��������򴴽�
	in<<":"<<inst.match(se)<<"\n";

	//in<<":"<<setw(2) <<setfill('0')<<setiosflags(ios::uppercase)<<hex<<machcode<<dec<<addr<<endl;
	in.close();//�ر��ļ�

}

FILE *fp_in;
FILE *fp_out;

//const char *fake[]={	"ORG","END","DB","EQU","DB","DW","DS","BIT"	};			//αָ��


int main()
{
	Inst inst;
	ofstream in;
	readasm(inst);
	string str;
	//	inst.int_to_string(256,5);			//����int_to_string
	//inst.string_to_int("FFF");				//����string_to_int
	//inst.get_checksum(mc);				//����get_checksum
	while(mc.length()>16)					//��������봮̫��������16�ֽھͷָ�
	{
		str=mc.substr(0,16);
		mc=mc.substr(16,mc.length());
		addr_s+=16;
		get_obj(str);
	}
	str=mc;
	addr_s+=mc.length();
	get_obj(str);
	in.open("obj.txt",ios::app);
	in<<":00000001FF"<<"\n";			//��ӽ������
	return 1;
}
