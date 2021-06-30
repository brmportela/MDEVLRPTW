#include <iostream>
#include <fstream>
#include<string>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <fcntl.h>
#include <windows.h>
#include <locale.h>

using namespace std;

BOOL DirectoryExists(LPCTSTR path)
{
  DWORD dwAttrib = GetFileAttributes(path);

  return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
         (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

void deletar_arquivo(string caminho) {

    DeleteFileA(caminho.c_str());
}

void deletar_pasta(string pasta) {

    RemoveDirectoryA(pasta.c_str());
}

bool file_exist(string fileName)
{
    ifstream infile(fileName.c_str());
    return infile.good();
}

int main()
{
    string pasta, arquivo, arquivo2, arquivo3, instance;
    FILE *arq;
	int a=1;
	int b=0;
	int x=0;
	int q = 36;
	string nomes[q] = {"c101C5", "c103C5", "c206C5", "c208C5", "r104C5", "r105C5", "r202C5", "r203C5", "rc105C5", "rc108C5", "rc204C5", "rc208C5", "c101C10", "c104C10", "c202C10", "c205C10", "r102C10", "r103C10", "r201C10", "r203C10", "rc102C10", "rc108C10", "rc201C10", "rc205C10", "c103C15", "c106C15", "c202C15", "c208C15", "r102C15", "r105C15", "r202C15", "r209C15", "rc103C15", "rc108C15", "rc202C15", "rc204C15"};
	while(x < q)
	{
		cout << "##############################################################################" << endl;
		cout << "-----------------------------------" << nomes[x] << "-----------------------------------" << endl;
		cout << "##############################################################################" << endl;

		pasta = "results/" + nomes[x];
		if(DirectoryExists(pasta.c_str()))
        {
            cout << "Diretorio existe!" << endl;
            deletar_pasta(pasta);
        }
        //mkdir(pasta.c_str());

		arquivo = pasta + ".txt";
		if(file_exist(arquivo)){
			deletar_arquivo(arquivo);
		}

		arquivo2 = "results/R_" + nomes[x] + ".txt";
		if(file_exist(arquivo2)){
			deletar_arquivo(arquivo2);
		}
		arq = fopen(arquivo2.c_str(),"w");


		arquivo3 = "results/RT_" + nomes[x] + ".txt";
		if(file_exist(arquivo3)){
			deletar_arquivo(arquivo3);
		}
		arq = fopen(arquivo3.c_str(),"w");

		b=0;
		/*while(b < a)
		{
		  cout << "---------------------------------------------------------------------------" << endl;
		  cout << "Instancia - " << nomes[x] << " / Iteracao " << b << endl;
		  instance = "start C:/Users/Bráulio/Projeto/projeto_principal.exe run C:/Users/Bráulio/Projeto/instance/" + nomes[x] + ".txt " + to_string(b) + " 100";
		  //printf(instance.c_str());
		  system(instance.c_str());
		  b++;
		}*/
		x++;
	}
}
