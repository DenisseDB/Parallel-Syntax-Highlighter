//CALCULA CUANTOS DIAS TIENE EL MES.
 
#include <iostream>
using namespace std;
int main()
{
    int mes,dias,a;
    cout<<"ingrese el mes y el a"<<char(164)<<"o"<<endl;
    cin>>mes>>a;
 
switch (mes)
{
case 1: case 3: case 5: case 7: case 8: case 10: case 12:
dias = 31;
break;
case 4: case 6: case 9: case 11:
dias = 30;
break;
case 2:if(a%4==0)
dias = 28;
    else
dias=29;
break;
default:
printf("El mes no es válido\n");
break;
}
cout<<dias<<endl;
cin.ignore();return 0;
  }