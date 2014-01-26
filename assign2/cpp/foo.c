
void kaa()
{
    printf("hello\n");
}

void blah()
{
    int x =3;
    while(x ==4)
    {
       kaa(); 
    }

}



int main() {
    	
    int  Grade = 'A';

     switch( Grade )
     {
        case 'A' : blah();
        case 'B' : printf( "Good\n" );
        case 'C' : printf( "OK\n" );
        case 'D' : printf( "Mmmmm....\n" );
        case 'F' : printf( "You must do better than this\n" );   
        case 'g' : printf( "Mmmmm....\n" );
        case 'h' : return;    
        case 'i' : return;
        case 'j' : printf( "You must do better than this\n" ); 
        case 'k' : printf( "Mmmmm....\n" );
        case 'f' : return;   
        default  : printf( "What is your grade anyway?\n" );
     }	
}

