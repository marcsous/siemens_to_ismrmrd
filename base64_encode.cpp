#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>


static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}
	return ret;
}


void splitBigString(std::ofstream &resFile, std::string bigString, int parts)
{
	int len = bigString.length();

	int at, pre=0, i;

	for (pre = i = 0; i < parts; ++i)
    {
		at = (len + len*i)/parts;
		if (parts-i == 1)
		{
			resFile << "\""<< bigString.substr(pre, at-pre) << "\");" << std::endl;
		}
		else
		{
			resFile << "\""<< bigString.substr(pre, at-pre) << "\"" << std::endl;
		}
		pre = at;
    }
}


int main(int argc, char *argv[])
{

	std::string infile_xml_VB(argv[1]);
	std::string infile_xml_VD(argv[2]);
	std::string infile_xsl(argv[3]);
	std::string infile_xsd(argv[4]);
	std::string encoded_file(argv[5]);
	//std::string decoded_file(argv[3]);

	std::ifstream t_xml_VB(infile_xml_VB.c_str());
	std::string str_xml_VB((std::istreambuf_iterator<char>(t_xml_VB)), std::istreambuf_iterator<char>());

	std::ifstream t_xml_VD(infile_xml_VD.c_str());
	std::string str_xml_VD((std::istreambuf_iterator<char>(t_xml_VD)), std::istreambuf_iterator<char>());

	std::ifstream t_xsl(infile_xsl.c_str());
	std::string str_xsl((std::istreambuf_iterator<char>(t_xsl)), std::istreambuf_iterator<char>());

	std::ifstream t_xsd(infile_xsd.c_str());
	std::string str_xsd((std::istreambuf_iterator<char>(t_xsd)), std::istreambuf_iterator<char>());

	std::string encoded_xml_VB = base64_encode(reinterpret_cast<const unsigned char*>(str_xml_VB.c_str()), str_xml_VB.length());
	std::string encoded_xml_VD = base64_encode(reinterpret_cast<const unsigned char*>(str_xml_VD.c_str()), str_xml_VD.length());
	std::string encoded_xsl = base64_encode(reinterpret_cast<const unsigned char*>(str_xsl.c_str()), str_xsl.length());
	std::string encoded_xsd = base64_encode(reinterpret_cast<const unsigned char*>(str_xsd.c_str()), str_xsd.length());

	std::ofstream ofs1 (encoded_file.c_str(), std::ofstream::out);

	ofs1 << "#include <iostream>" << std::endl << std::endl;

	ofs1 << "std::string global_xml_VB_string(";

	splitBigString(ofs1, encoded_xml_VB, 4);

	ofs1 << "std::string global_xml_VD_string(";

	splitBigString(ofs1, encoded_xml_VD, 4);

	ofs1 << "std::string global_xsl_string(";

	splitBigString(ofs1, encoded_xsl, 4);

	ofs1 << "std::string global_xsd_string(";

	splitBigString(ofs1, encoded_xsd, 4);

	//ofs1 << "#include <iostream>" << std::endl << std::endl << "std::string global_xml_VB_string(\"" << encoded_xml_VB << "\");" << std::endl << "std::string global_xml_VD_string(\"" << encoded_xml_VD << "\");" << std::endl << "std::string global_xsl_string(\"" << encoded_xsl << "\");" << std::endl << "std::string global_xsd_string(\"" << encoded_xsd << "\");" << std::endl;

	ofs1.close();

	return 0;
}