#include <fstream>
#include <iostream>
using namespace std;

class OS
{
public:
    char M[100][4]; // Physical Memory
    char IR[4];     // Instruction Register (4 bytes)
    char R[4];      // General Purpose Register (4 bytes)
    int IC;         // Instruction Counter Register (2 bytes)
    int SI;         // Interrupt
    bool C;         // Toggle (1 byte)
    char buffer[40];
    fstream infile;
    fstream outfile;

    OS(){
        infile.open("input.txt", ios::in);
        init();
    }

// initiallise everything to 0
    void init()
    {

        for (int i = 0; i < 100; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                M[i][j] = ' ';
            }
        }

        IR[0] = {' '};
        R[0] = {' '};
        C = false;
    }

// Master Mode
    void MOS()
    {

        if (SI == 1) // Read Mode
        {
            for (int i = 0; i <= 39; i++)
                buffer[i] = '\0';

            infile.getline(buffer, 40);

            int k = 0;
            int i = IR[2] - 48;
            i = i * 10;

            for (int l = 0; l < 10; ++l)
            {
                for (int j = 0; j < 4; ++j)
                {
                    M[i][j] = buffer[k];
                    k++;
                }
                if (k == 40)
                {
                    break;
                }
                i++;
            }

            for (int i = 0; i < 100; i++)
            {
                cout << "M[" << i << "]\t";
                for (int j = 0; j < 4; j++)
                {
                    cout << M[i][j];
                }
                cout << endl;
            }
        }
        else if (SI == 2) // Write Mode
        {
            outfile.open("out.txt", ios::app);

            for (int i = 0; i <= 39; i++)
                buffer[i] = '\0';

            int k = 0;
            int i = IR[2] - 48;
            i = i * 10;

            for (int l = 0; l < 10; ++l)
            {
                for (int j = 0; j < 4; ++j)
                {
                    buffer[k] = M[i][j];
                    if (buffer[k]!='\0'){
                        outfile << buffer[k];
                    }
                    k++;
                }
                if (k == 40)
                {
                    break;
                }
                i++;
            }
            for (int i = 0; i < 100; i++)
            {
                cout << "M[" << i << "]\t";
                for (int j = 0; j < 4; j++)
                {
                    cout << M[i][j];
                }
                cout << endl;
            }

            outfile << "\n";
            outfile.close();
        }
        else if (SI == 3) // Terminate
        {
            outfile.open("out.txt", ios::app);

            outfile << "\n";
            outfile << "\n";
            outfile.close();
        }
    }

// Execution
    void Execute()
    {
        while (true)
        {
            for (int i = 0; i < 4; i++) // Load in register
            {
                IR[i] = M[IC][i];
            }
            IC++;

            if (IR[0] == 'G' && IR[1] == 'D') // GD
            {
                SI = 1;
                MOS();
            }
            else if (IR[0] == 'P' && IR[1] == 'D') // PD
            {
                SI = 2;
                MOS();
            }
            else if (IR[0] == 'H') // H
            {
                SI = 3;
                MOS();
                break;
            }
            else if (IR[0] == 'L' && IR[1] == 'R') // LR
            {
                int i = IR[2] - 48;
                i = i * 10 + (IR[3] - 48);

                for (int j = 0; j <= 3; j++)
                    R[j] = M[i][j];

                // for(int j=0;j<=3;j++)
                //  cout<<R[j];

                cout << endl;
            }
            else if (IR[0] == 'S' && IR[1] == 'R') // SR
            {
                int i = IR[2] - 48;
                i = i * 10 + (IR[3] - 48);
                // cout<<i;
                for (int j = 0; j <= 3; j++)
                    M[i][j] = R[j];

                cout << endl;
            }
            else if (IR[0] == 'C' && IR[1] == 'R') // CR
            {
                int i = IR[2] - 48;
                i = i * 10 + (IR[3] - 48);
                // cout<<i;
                int count = 0;

                for (int j = 0; j <= 3; j++)
                    if (M[i][j] == R[j])
                        count++;

                if (count == 4)
                    C = true;

                // cout<<C;
            }
            else if (IR[0] == 'B' && IR[1] == 'T') // BT
            {
                if (C == true)
                {
                    int i = IR[2] - 48;
                    i = i * 10 + (IR[3] - 48);

                    IC = i;
                }
            }
        }
    }

// Load Function
    void LOAD()
    {

        cout << "Reading Data..." << endl;
        int x = 0;
        do
        {
            for (int i = 0; i <= 39; i++) // clear buffer
                buffer[i] = '\0';

            infile.getline(buffer, 40);

            for (int k = 0; k <= 39; k++)
                cout << buffer[k];

            if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
            {
                init();
            }
            else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
            {
                IC = 00;
                Execute();
            }
            else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
            {
                x = 0;
                continue;
            }
            else
            {
                int k = 0;

                for (; x < 100; ++x)
                {
                    for (int j = 0; j < 4; ++j)
                    {

                        M[x][j] = buffer[k];
                        k++;
                    }

                    if (k == 40 || buffer[k] == ' ' || buffer[k] == '\n')
                    {
                        break;
                    }
                }
            }

        } while (!infile.eof()); // continues to take input till eof
    }
};
int main()
{
    OS os;


    os.LOAD();

    return 0;
}
