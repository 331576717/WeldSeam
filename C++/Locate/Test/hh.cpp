#include<iostream>
#include<sstream>
#include<string>
using namespace std;
int main()
{
	int intCount=1;
	string s="https://class.coursera.org/images-002/lecture/subtitles?";
	while(intCount < 174)
	{
		stringstream streamCount;
		streamCount << intCount;
		string stringCount;
		streamCount >> stringCount;
		string res = s + "q=" + stringCount + "_" + "en" + "&format=srt";
		cout << res << endl;;
		res = s + "q=" + stringCount + "_" + "zh" + "&format=srt";
		cout << res << endl;
		intCount+=2;
	}
	https://class.coursera.org/images-002/lecture/subtitles?q=173_en&format=srt
	
	getchar();
}