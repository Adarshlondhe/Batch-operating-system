#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

char IR[4], R[4];
int CMP, IC , SI, PI, TI;
int i, j, k;
char Mem[300][4];
char buff[40];
int location;
int pages[30];
int ptr; // page table register
int page_table_ptr;

int ttl, ttc, tll, llc;


void LOAD();
void INIT();
void START_EXECUTION();
void EXECUTE();
void READ();
void WRITE();
void TERMINATE();
void MOS();

fstream newfile;
fstream newofile;

void amjParsing() {

    int z = 0;

    while(z<40 && buff[z] != '0') {

        if(buff[z] == 'G' && buff[z+1] == 'D') {

            cout<<"in g"<<endl;

            ttc += 2;
            z+=4;
        }

        else if(buff[z] == 'S' && buff[z+1] == 'R') {

            ttc += 2;
            z+=4;
            cout<<"in s"<<endl;

        }

        else if(buff[z] == 'P' && buff[z+1] == 'D') {

            ttc += 1;
            llc += 1;
            z+=4;
            cout<<"in p"<<endl;

        }

        else if(buff[z] == 'L' && buff[z+1] == 'R') {

            ttc += 1;
            z += 4;
            cout<<"in l"<<endl;

        }

        else if(buff[z] == 'B' && buff[z+1] == 'T') {

            ttc += 1;
            z += 4;
            cout<<"in b"<<endl;

        }

        else if(buff[z] == 'C' && buff[z+1] == 'R') {

            ttc += 1;
            z += 4;
            cout<<"in c"<<endl;

        }

        else if(buff[z] == 'H') {

            ttc += 1;
            z += 1;
            cout<<"in z"<<endl;

        }

        else {

            break;
        }
    }

}

void LOAD()
{
    int i,j,no=0,a=0;

    if (newfile.is_open()){
      string tp;
      i=0;

      while(getline(newfile, tp)){
          for(i=0;i<tp.length();i++){
              buff[i]=tp[i];
          }

          if (buff[0] == '$' && buff[1] == 'A' && buff[2] == 'M' && buff[3] == 'J'){
            cout<<"AMJ COMMAND FOUND:-"<<endl;

            string tt; // time limit
            tt.push_back(buff[8]);
            tt.push_back(buff[9]);
            tt.push_back(buff[10]);
            tt.push_back(buff[11]);

            ttl = stoi(tt);

            string ll; // line limit
            ll.push_back(buff[12]);
            ll.push_back(buff[13]);
            ll.push_back(buff[14]);
            ll.push_back(buff[15]);

            tll = stoi(ll);

            ttc = 0;

            llc = 0;

            no = 0;
            INIT();

            ptr = rand()%30;

            cout<<"ptr-> "<<ptr<<endl;

            page_table_ptr = ptr*10;

            pages[ptr] = 1;

            for(i = page_table_ptr; i<page_table_ptr+10; i++) {  // initializing page table to *

                for(j = 0; j<4; j++) {

                    Mem[i][j] = '*';
                }
            }

            cout<<"Printing the instruction register content: "<<endl;
            for (i = 0; i < 4; i++){
                cout<<IR[i];
            }
            cout<<endl;
            cout<<"Printing the content of general purpose register: "<<endl;
            for (i = 0; i < 4; i++){
                cout<<R[i];
            }
            cout<<endl;
            cout<<"Printing the Compare flag"<<endl;
            cout<<CMP;
            cout<<endl;
            cout<<"Printing the instruction counter"<<endl;
            cout<<IC<<endl;
        }
        else if (buff[0] == '$' && buff[1] == 'D' && buff[2] == 'T' && buff[3] == 'A'){
            for (i = 0; i < 40; i++)
            {
                buff[i] = '\0';
            }
            cout<<"Starting Execution:- "<<endl;
            START_EXECUTION();

        }
        else if (buff[0] == '$' && buff[1] == 'E' && buff[2] == 'N' && buff[3] == 'D'){

            for(int i = 0; i<300; i++) {

                cout<<"Mem["<<i<<"]: ";

                for(int j = 0; j<4; j++) {

                    cout<<Mem[i][j];
                }

                cout<<endl;
            }

            cout<<"ttc -> "<<ttc<<endl;
            cout<<"ttl -> "<<ttl<<endl;
            cout<<"llc -> "<<llc<<endl;
            cout<<"tll -> "<<tll<<endl;

            cout<<"$END COMMAND FOUND."<<endl;
            cout<<"Process Ended.\n"<<endl;
            continue;
        }
        else {

            int k = 0;

            while(buff[k] != '\0' && k<40) {



                int temp = rand()%30;

                if(pages[temp] == 1) {

                    continue;
                }

                else {

                    pages[temp] = 1;

                    string s1 = to_string(temp);

                    if(s1.length() == 1) {

                        Mem[page_table_ptr][0] = '\0';
                        Mem[page_table_ptr][1] = '\0';
                        Mem[page_table_ptr][2] = '0';
                        Mem[page_table_ptr][3] = s1[0];
                    }

                    else {

                        Mem[page_table_ptr][0] = '\0';
                        Mem[page_table_ptr][1] = '\0';
                        Mem[page_table_ptr][2] = s1[0];
                        Mem[page_table_ptr][3] = s1[1];
                    }

                    page_table_ptr++;

                    for (i = (temp*10); i < (temp*10) + 10; i++){
                        for (j = 0; j < 4; j++){
                            Mem[i][j] = buff[k++];
                        }
                    }

                    for(i=0;i<40;i++){
                        buff[i]='\0';

                    }
                }
            }
        }
    }

    newfile.close(); //close the file object.
    newofile.close();

}
}

void INIT(){
    for (i = 0; i < 300; i++){
        for (j = 0; j < 4; j++)
            Mem[i][j] = '\0';
    }

    for (i = 0; i < 4; i++){
        IR[i] = R[i] = '\0';
    }
    CMP = IC = 0;

    PI = TI = SI = 0;


}

int address_map(int VA) {

    int pte = (ptr*10) + VA/10;

    if(Mem[pte][0] == '*' || Mem[pte][1] == '*' || Mem[pte][2] == '*' || Mem[pte][3]== '*')
    {

        cout<<"Page Fault"<<endl;
        return -1;
    }

    else {

        string aaa;

        aaa.push_back(Mem[pte][2]);
        aaa.push_back(Mem[pte][3]);

        int abc = stoi(aaa);

        return abc*10 + VA%10;

    }

}

void MOS()
{
    if(PI==1)
    {

        cout<<"\n** Opcode Error : \n Program terminated abnormally. **\n\n";
            newofile<<"* Opcode error : Program terminated abnormally. *\n";
            newofile<<"TTC->"<<ttc<<" TTL->"<<tll<<" TLL->"<<tll<<" LLC->"<<llc<<" SI->"<<SI<<" PI->"<<PI<<" TI->"<<TI;
            TERMINATE();
            return;
    }
    else if(PI==2)
    {   //cout<<"hellooooooooooooooooooo"<<endl;
        cout<<"\n** Operand Error : \n Program terminated abnormally. **\n\n";
            newofile<<"* Operand error :  Program terminated abnormally. *\n";
            newofile<<"TTC->"<<ttc<<" TTL->"<<tll<<" TLL->"<<tll<<" LLC->"<<llc<<" SI->"<<SI<<" PI->"<<PI<<" TI->"<<TI;
            TERMINATE();
            return;
    }
    else if(PI==3) {
        cout<<"\n** Invalid Page Fault :  Program terminated abnormally. **\n\n";
            newofile<<"* Invalid Page Fault :  Program terminated abnormally. *\n";
            newofile<<"TTC->"<<ttc<<" TTL->"<<tll<<" TLL->"<<tll<<" LLC->"<<llc<<" SI->"<<SI<<" PI->"<<PI<<" TI->"<<TI;
            TERMINATE();
            return;
    }


    if(SI==3)
        TERMINATE();
    else if(SI==1)
    {
        if(TI==0)
            READ();
        else if(TI==2)
        {
            cout<<"\n** Time Limit Exceeded : \n Program terminated abnormally. **\n";
            newofile<<"* Time Limit Exceeded :  Program terminated abnormally. *\n";
            newofile<<"TTC->"<<ttc<<" TTL->"<<tll<<" TLL->"<<tll<<" LLC->"<<llc<<" SI->"<<SI<<" PI->"<<PI<<" TI->"<<TI;
            TERMINATE();
        }
    }
    else if(SI==2)
    {
        cout<<"\nIN SI=2";
        if(TI==0)
        {
            cout<<"\nIN TI=0";
            WRITE();
        }
        else if(TI==2)
        {
            cout<<"\nIN TI=0";
            //WRITE();
            cout<<"\n** Time Limit Exceeded :  Program terminated abnormally. **";
            newofile<<"* Time Limit Exceeded : Program terminated abnormally. *\n";
            newofile<<"TTC->"<<ttc<<" TTL->"<<tll<<" TLL->"<<tll<<" LLC->"<<llc<<" SI->"<<SI<<" PI->"<<PI<<" TI->"<<TI;
            TERMINATE();


        }
    }
}

void examine()
{
    char ch;
    ch=IR[0];
    PI=0;

    switch(ch)

    {
    case 'G':
        cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
        cout<<"TTC = "<<ttc<<"  LLC="<<llc<<endl;

        if(IR[1] !='D')
        {
            PI=1;
            MOS();
            return;
        }
        else
        {
            if(ttc <= ttl)
            {
                SI=1;
                MOS();

            }
        }
        cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
        cout<<"TTC = "<<ttc<<"  LLC="<<llc<<endl;
        break;

    case 'P':
        SI=2;
        cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
        cout<<"TTC = "<<ttc<<"  LLC="<<llc<<endl;
        if(IR[1] != 'D')
        {
            PI=1;
            MOS();
            return;
        }
        else
        {

            if(llc <= tll)
            {
                TI=0;
                SI=2;
                MOS();

            }
        }
        cout<<"\nTTC = "<<ttc<<"  LLC="<<llc<<endl;
        cout<<"SI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
        break;


    case 'H':

        SI=3;
        MOS();
        break;
    }


}

void START_EXECUTION(){
    IC = 0;
    fstream obj1;
    string ending;
    obj1.open("input_load_1.txt",ios::in);
    obj1.seekg(newfile.tellp(), ios::beg);
    getline(obj1, ending);
    if(ending[0]=='$' && ending[1] == 'E' && ending[2] == 'N' && ending[3] == 'D') {

         cout<<"\n** No data error : \n Program terminated abnormally. **";
            newofile<<"* No data error : Program terminated abnormally. *\n";
            newofile<<"TTC->"<<ttc<<" TTL->"<<tll<<" TLL->"<<tll<<" LLC->"<<llc<<" SI->"<<SI<<" PI->"<<PI<<" TI->"<<TI;
            obj1.close();
            TERMINATE();
    }
    else {

        obj1.close();
        EXECUTE();
    }

}

void EXECUTE(){
    cout<<endl;

    int RA = address_map(IC);

    for (i = 0; i < 4; i++){
        IR[i] = Mem[RA][i];
    }
    for (i = 0; i < 4; i++){
        cout<<IR[i];
    }

    if((int)IR[2]<48 || (int)IR[2]>57 || (int)IR[3]<48 || (int)IR[3]>57) {
        PI = 2;
        MOS();
        return;
    }

    location = ((int)IR[2] - 48) * 10 + ((int)IR[3] - 48);

    IC++;

    if (IR[0] == 'L' && IR[1] == 'R'){

        ttc++;

        if(ttc>ttl) {

            TI = 2;
            MOS();
            return;
        }

        cout<<"\nLR command - "<<endl;
        cout<<"Register Values: ";
        for (i = 0; i < 4; i++)
        {
            R[i] = Mem[location][i];
            cout<<R[i];
        }
        cout<<endl;
        EXECUTE();
    }
    else if (IR[0] == 'S' && IR[1] == 'R'){

        ttc += 2;

        if(ttc>ttl) {

            TI = 2;
            MOS();
            return;
        }

        cout<<"\nSR command - "<<endl;
        cout<<"\ndata at loc "<<location<< " is :- ";
        for (i = 0; i < 4; i++){
            Mem[location][i] = R[i];
            cout<<Mem[location][i];
        }
        EXECUTE();
    }

    else if (IR[0] == 'C' && IR[1] == 'R'){

        ttc++;

        if(ttc>ttl) {

            TI = 2;
            MOS();
            return;
        }

        cout<<"\nCR command - "<<endl;
        for (i = 0; i < 4; i++){
            if (R[i] == Mem[location][i]){
                CMP = 1;
            }

            else{
                CMP = 0;
                break;
            }
        }
        if (CMP) {
            cout<<"\nStrings ARE SAME"<<endl;
        }
        else {
            cout<<"\nStrings ARE NOT SAME"<<endl;
        }

        EXECUTE();
    }
    else if (IR[0] == 'B' && IR[1] == 'T')
    {
        ttc++;

        if(ttc>ttl) {

            TI = 2;
            MOS();
            return;
        }

        cout<<"\nBT command - "<<endl;
        if (CMP)
        {
            IC = location;
            cout<<"Value of IC - "<<IC;
        }
        EXECUTE();
    }
    else if (IR[0] == 'G' && IR[1] == 'D')
    {

        ttc+=2;

        if(ttc>ttl) {

            TI = 2;
            SI = 1;
            MOS();
            return;
        }

        int temp1 = address_map(location);
        if(temp1 == -1) {

            int r1 = rand()%30;


            if(pages[r1] == 1) {

                while(pages[r1] == 1) {

                    r1 = rand()%30;
                }

                pages[r1] = 1;

                    string s1 = to_string(r1);

                    if(s1.length() == 1) {

                        Mem[page_table_ptr][0]='\0';
                        Mem[page_table_ptr][1]='\0';
                        Mem[page_table_ptr][2] = '0';
                        Mem[page_table_ptr][3] = s1[0];

                        page_table_ptr++;
                    }

                    else {

                        Mem[page_table_ptr][0] = '\0';
                        Mem[page_table_ptr][1] = '\0';
                        Mem[page_table_ptr][2] = s1[0];
                        Mem[page_table_ptr][3] = s1[1];

                        page_table_ptr++;
                    }
            }
            else {

                pages[r1] = 1;

                    string s1 = to_string(r1);

                    if(s1.length() == 1) {
                        Mem[page_table_ptr][0]='\0';
                        Mem[page_table_ptr][1]='\0';
                        Mem[page_table_ptr][2] = '0';
                        Mem[page_table_ptr][3] = s1[0];

                        page_table_ptr++;
                    }

                    else {

                        Mem[page_table_ptr][0] = '\0';
                        Mem[page_table_ptr][1] = '\0';
                        Mem[page_table_ptr][2] = s1[0];
                        Mem[page_table_ptr][3] = s1[1];

                        page_table_ptr++;
                    }
                }
        }
        examine();
    }
    else if (IR[0] == 'P' && IR[1] == 'D')
    {

        ttc++;

        if(ttc>ttl) {

            SI = 2;
            TI = 2;
            MOS();
            return;
        }

        llc++;

        if(llc>tll) {

            cout<<"\n** Line Limit Exceeded : \n Program terminated abnormally. **";
            newofile<<"* Line Limit Exceeded : Program terminated abnormally. *\n";
            newofile<<"TTC->"<<ttc<<" TTL->"<<tll<<" TLL->"<<tll<<" LLC->"<<llc<<" SI->"<<SI<<" PI->"<<PI<<" TI->"<<TI;
            TERMINATE();
        }

        int temp1 = address_map(location);
        if(temp1 == -1) {

            cout<<"* invalid page fault.\n Program terminated abnormally\n\n";
            PI = 3;
            TI = 0;
            MOS();
            return;

        }
        else {

            cout<<IR[0]<<IR[1]<<IR[2]<<IR[3]<<endl;
            examine();
            return;
        }

    }
    else if (IR[0] == 'O' && IR[1] == 'R'){
        int res,i=0;
        string s1,s2;
        for(i=0;i<4 && R[i]!='\0';i++){
            s1.push_back(R[i]);
        }
        int num1=stoi(s1);
        for(i=0;i<4;i++){
            s2.push_back(Mem[location][i]);
        }
        int num2=stoi(s2);
        res=(num1 | num2);
        cout<<"The OR of "<<num1<<" and "<<num2<<" is "<<res<<endl;
        for(i=0;i<4;i++){
            Mem[location][i]='\0';
        }
        string result=to_string(res);

        for(i=0;i<result.length();i++){
            Mem[location][i]=result[i];
        }
        EXECUTE();
    }
    else if (IR[0] == 'A' && IR[1] == 'D'){
        int res,i=0;
        string s1,s2;
        for(i=0;i<4 && R[i]!='\0';i++){
            s1.push_back(R[i]);
        }
        cout<<s1<<endl;
        int num1=stoi(s1);
        for(i=0;i<4;i++){
            s2.push_back(Mem[location][i]);
        }
        cout<<s2<<endl;
        int num2=stoi(s2);
        res=(num1 & num2);
        cout<<"The AND of "<<num1<<" and "<<num2<<" is "<<res<<endl;
        for(i=0;i<4;i++){
            Mem[location][i]='\0';
        }
        string result=to_string(res);

        for(i=0;i<result.length();i++){
            Mem[location][i]=result[i];
        }
        EXECUTE();
    }
    else if (IR[0] == 'H')
    {
        ttc++;

        if(ttc>ttl) {

            TI = 2;
            MOS();
            return;
        }

        SI = 3;
        PI = 0;
        cout<<"\nProgram Terminated"<<endl;
        MOS();
        return;
    }

    else {

        PI = 1;
        MOS();
        return;

    }
}

void READ(){

        string z;
        getline(newfile, z);
        for(i=0;i<z.length();i++){
            buff[i]=z[i];
        }
        for(int i=0;i<40;i++)
            cout<<buff[i]<<" ";
        cout<<endl;
        k=0;
        int x = address_map(location);
        for (i = x; i < x + 10; i++)
        {
            for (j = 0; j < 4 && buff[k] != '\0'; j++)
            {
                Mem[i][j] = buff[k];
                k++;
            }
        }
        for (i = 0; i < 40; i++)
        {
            buff[i] = '\0';
        }

    EXECUTE();
}

void WRITE(){
    k = 0;
    cout<<endl;
    for (i = 0; i < 40; i++)
    {
        buff[i]='\0';
    }
    int temp1 = address_map(location);
    for (i = temp1; i < temp1 + 10; i++)
    {
        for (j = 0; j < 4; j++)
        {
            buff[k] = Mem[i][j];
            k++;
        }
    }


    cout<<endl;
    string s;
    if (newofile.is_open()) {

        s="";
        int i=0;
        while(buff[i]!='\0'){
            s.push_back(buff[i]);
            i++;
        }

        newofile<<s;
    }
    for (i = 0; i < 40; i++){
        buff[i] = '\0';
    }

    EXECUTE();
}

void TERMINATE(){
    newofile<<"\n\n\n";
}

int main(){
   newfile.open("input_load_1.txt",ios::in);
   newofile.open("output.txt",ios::out);

   LOAD();
}
