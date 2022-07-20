// =================================================================
//
// Resaltador de sintaxis paralelo
// Author: Denisse Dominguez Bolaños-
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

const int THREADS = 8;

using namespace std;

// =========================== SEQUENTIAL ==========================
string unHilo(string archivo) {
  fstream pruebas;
  string resultados, highlight;
  vector<char>::iterator it;
  pruebas.open(archivo, ios::in);

  while(getline(pruebas, resultados)){
    // Definimos los operadores aritmeticos
    string variable = "[a-zA-Z][a-zA-Z_0-9]*";
    string operadores = "[\\*|\\/|\\^|\\=|\\+|\\-]";
    string numero = "-*[0-9]+\\.[0-9]+([E][-*][0-9]+)?|-*[0-9]+(\\.[0-9]+)?";
    //string especiales = "[\\(\\)]";
    string comentarios = " //.*$"; 
    string reservada = "main|iostream|#include|using|namespace|std|cout|cin|else|long|switch|break|typedef|return|char|float|const|for|sizeof|if|while|do|int|double|\\/\\/.*|\".*\"|^\\-*[0-9]+| ^[a-zA-Z]+|\\^|\\/|\\&|\\|\\+|\\-|\\*|\\=|\\(|\\)|\\=>|\\<=|.";
    regex regex_tokens(variable +"|"+ operadores +"|"+ numero +"|"+ comentarios +"|"+ reservada); 

    // Definimos nuestro iterador para ir recorriendo y buscar en regex los tokens
    // Iterador para ir recorriendo match por match y detectarlos en regex_tokens
    auto inicio_arch = 
    // leer de resultados incio a resultados final 
    std::sregex_iterator(resultados.begin(), resultados.end(), regex_tokens); 
    // cuando ya no tenga mas elementos se deja de leer 
    auto fin_arch = std::sregex_iterator(); 

    // Vamos a ir leyendo caracter x caracter, al detectar su token, pasamos al siguiente carcater
    for (std::sregex_iterator i = inicio_arch; i != fin_arch; ++i) {
      std::smatch match = *i;                                              
      std::string caracter = match.str(); 
      // Ya que tebemos el string, vemos si esta hace match con el regex, que basta solo con poner regex ya que se llama al constructor
      if(regex_match(caracter, regex(variable))){ 
        highlight = highlight + "<span class=\"variable\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(numero))){
        highlight = highlight + "<span class=\"numero\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(comentarios))){
        highlight = highlight + "<span class=\"comentario\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(operadores))){
        highlight = highlight + "<span class=\"operador\">" + caracter + "</span>";
      }else if(regex_match(caracter, regex(reservada))){
        highlight = highlight + "<span class=\"reservada\">" + caracter + "</span>";
      }
    }
  }
  return highlight;
}

// =================================================================

// =========================== CONCURRENT ==========================
typedef struct {
    string *array;
} Block;

void* variosHilos (void* param) {
  Block *block;

  block = (Block*) param;
  for (int i = block->start; i < block->end; i++) {
    
  }
  pthread_exit(0);
} 
