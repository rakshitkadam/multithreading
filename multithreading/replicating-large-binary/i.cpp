#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void replicateBinaries (const string& sourcePath, const string& destPath, size_t chunkSize) 
{
	ifstream src(sourcePath, ios::binary);
	if(!src) 
	{
		std::cerr<<"Error,cant open source file";
		return;
	}
	ofstream dst(destPath, ios::binary);
	vector<char>buffer(chunkSize);
	if(!dst) 
	{
		cerr<<"Destination file doesn't exist";
		return;
	}
	while(src) 
	{
		src.read(buffer.data(), buffer.size());
		std::streamsize bytesRead = src.gcount(); //how many bytes were actually read in the last read() call
		if(bytesRead > 0) 
		{
			dst.write(buffer.data(), bytesRead);
		}
	}
}
int main() 
{
	return 0;
}


#include <fstream>
#include <iostream>
#include <vector>

// void replicateBinary(const std::string& sourcePath, const std::string& destPath, size_t chunkSize) {
//     std::ifstream src(sourcePath, std::ios::binary);
//     if (!src) {
//         std::cerr << "Error: Cannot open source file " << sourcePath << "\n";
//         return;
//     }

//     std::ofstream dst(destPath, std::ios::binary);
//     if (!dst) {
//         std::cerr << "Error: Cannot open destination file " << destPath << "\n";
//         return;
//     }

//     std::vector<char> buffer(chunkSize);

//     while (src) {
//         src.read(buffer.data(), buffer.size());
//         std::streamsize bytesRead = src.gcount();
//         if (bytesRead > 0) {
//             dst.write(buffer.data(), bytesRead);
//         }
//     }

//     src.close();
//     dst.close();
// }
