#include <fstream>
#include <vector>

using namespace std;

vector<string> files = {"users.txt", "text1.txt", "text2.txt"};

vector<string> contents;

void serial_concat() {
	contents.resize(files.size());
	for (int i = 0; i < files.size(); ++i) {
		string file_name = files[i];
		file_name = "../" + file_name;
		ifstream ifs(file_name);
		string line;
		string total_file_contents;
		while (getline(ifs, line)) {
			total_file_contents += line;
			total_file_contents += "\n";
		}
		contents[i] = total_file_contents;
	}

	ofstream outFile("../serial_concat.txt");
	for (const auto & content : contents) {
		outFile << content;
	}
	outFile.close();
}

void parallel_concat() {
	contents.resize(files.size());
	string total_file_contents;
	string line;
#pragma omp parallel shared(contents, files) private(total_file_contents, line)
	{
#pragma omp for schedule(static)
		for (int i = 0; i < files.size(); ++i) {
			string file_name = files[i];
			file_name = "../" + file_name;
			ifstream ifs(file_name);
			while (getline(ifs, line)) {
				total_file_contents += line;
				total_file_contents += "\n";
			}

			contents[i] = total_file_contents;
		}

#pragma omp critical
		{
			ofstream outFile("../parallel_concat.txt");
			for (const auto & content : contents)
				outFile << content;
			outFile.close();
		}
	}
}

int main() {
	serial_concat();
	parallel_concat();
}

/*
 * Question 1:
 *	 Yes, it certainly seems like reduce needs to be commutative. If one thread
 *		does a computation that's later in the array of whatever is being worked on
 *		than what the current thread is working on, we would hope that the result is
 *		the same whichever thread is worked on first (say, in our dot product example).
 *		With the dot product, both multiplication and addition are commutative, if they
 *		were not, we would run into several troubles from running the program one time
 *		versus another.
 *
 *		An example of where this would matter is in number 2 for the homework. The
 *		files need to be concatenated in the order that they appear in the list. This
 *		is certainly not a commutative operation, as 1 needs to always come before 2 for
 *		the result to be correct according to the problem definition.
 */