// =================================================================
//
// Resaltador de sintaxis paralelo
// Author: Denisse Dominguez Bolaños
//
// =================================================================

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <pthread.h>
#include "utils.h"
#include <regex>
// mis include
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;

const int THREADS = 8;

// =========================== SEQUENTIAL ==========================
string unHilo(string archivo) {
  fstream buscar;
  string matches, highlight;
  vector<char>::iterator it;
  buscar.open(archivo, ios::in);

  while(getline(buscar, matches)){
    // Definimos los operadores aritmeticos
    string variables = "[a-zA-Z][a-zA-Z0-9_]*";
    string operadores = "\\/|\\+|\\-|\\*|\\=|\\(|\\)|\\^|\\<<|\\{|\\}|\\>>|\\<=|\\>=|\\;";
    string comentarios = "\\/\\/.*";
    string numero = "\\-*[0-9]+";
    string reservada = "#include|iostream|cin|printf|auto|else|endl|switch|break|enum|cout|typedef|string|extern|return|union|char|float|using|namespace|const|for|main|void|continue|goto|sizeof|volatile|default|if|stati|while|do|int|struct_Packed|double";
    string strings = "\".*\"";
    regex regex_tokens(comentarios +"|"+ strings +"|"+ reservada +"|"+ numero +"|"+ reservada +"|"+ variables  +"|"+ operadores + "|.");

    // Definimos nuestro iterador para ir recorriendo y buscar en regex los tokens
    // Iterador para ir recorriendo match por match y detectarlos en regex_tokens
    auto inicio_archivo =
    // leer de resultados incio a resultados final
    std::sregex_iterator(matches.begin(), matches.end(), regex_tokens);
    // cuando ya no tenga mas elementos se deja de leer
    auto fin_archivo = std::sregex_iterator();

    // Vamos a ir leyendo caracter x caracter, al detectar su token, pasamos al siguiente carcater
    for (std::sregex_iterator i = inicio_archivo; i != fin_archivo; ++i) {
      std::smatch match = *i;
      std::string caracter = match.str();
      // Ya que tebemos el string, vemos si esta hace match con el regex, que basta solo con poner regex ya que se llama al constructor
      if(regex_match(caracter, regex(comentarios))){
        highlight = highlight + "<span class=\"comentario\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(reservada))){
        highlight = highlight + "<span class=\"reservada\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(numero))){
        highlight = highlight  + "<span class=\"numero\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(variables))){
        highlight = highlight + "<span class=\"variable\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(operadores))){
        highlight = highlight + "<span class=\"operador\">" + caracter  + "</span>";
      }else{
        highlight = highlight + "<span class=\"string\">" + caracter  + "</span>";
      }
    }
  }
  return highlight;
}

// =================================================================


// =========================== CONCURRENT ==========================
//ifstream para leer entradas y sacar cosas
typedef struct {
    string file;
} Block;

void* variosHilos (void* param) {
    const string htmlStart = "<!DOCTYPE html><html lang='esp-mx'> <head> <meta charset='UTF-8'> <link rel=\"stylesheet\" href=\"./styles_AS.css\"> <title>Resaltador de Sintaxis Paralelo </title></head><body>";
    const string htmlEnd = "</body></html>";

    Block *block;
    block = (Block*) param;

    string result = htmlStart + unHilo(block->file) + htmlEnd;

    ofstream archivo;
    archivo.open("ResultadoParalelo-" + block->file + ".html");
    archivo << result;
    archivo.close();

    pthread_exit(0);
}

// ===========================================================

// ===========================================================
int main(int argc, char* argv[]) {
  const string htmlStart = "<!DOCTYPE html><html lang='esp-mx'> <head> <meta charset='UTF-8'> <link rel=\"stylesheet\" href=\"./styles_AS.css\"> <title>Resaltador de Sintaxis Paralelo </title></head><body>";
  const string htmlEnd = "</body></html>";

  //argv es un arreglo de strings
  double tiempoSecuencial, tiempoConcurrente;
  int blockSize;

  // argc es la cantidad de elementos a recibir
  // por lo tonto lo consideramos el tamaño de
  // entrada
  int tamanoEntrada = argc;

  // ========= HTML SEQUENTIAL ==============
  tiempoSecuencial = 0;
  start_timer();

  for (int i = 1; i < argc; i++) {
    string result = htmlStart + unHilo(argv[i]) + htmlEnd;

    // realizamos el archivo de salida
    // lo generamos con ofstream
    ofstream archivo;
    archivo.open("ResultadoSecuencial-" + string(argv[i]) + ".html");
    archivo << result;
    archivo.close();

  }
  tiempoSecuencial = stop_timer();

  cout << "Tiempo en secuencial = " << setprecision(5) << (tiempoSecuencial) <<" ms" << endl;

  // ========= CONCURRENT ==========
  tiempoConcurrente = 0;
  start_timer();

  pthread_t tids[argc];
  Block blocks[argc];

  for (int i = 1; i < argc; i++) {
    blocks[i].file = argv[i];
  }

  for (int i = 1; i < argc; i++) {
     pthread_create(&tids[i], NULL, variosHilos, (void*) &blocks[i]);
   }

 for (int i = 1; i < argc; i++) {
   pthread_join(tids[i], NULL);
 }

  tiempoConcurrente = stop_timer();
  cout << "Tiempo en paralelo = " << setprecision(5) << (tiempoConcurrente) <<" ms" << endl;

  return 0;
}
