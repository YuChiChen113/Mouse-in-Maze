//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include <stdio.h>
#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"

# include <stdlib.h>
# include <time.h>
# include <iostream>

#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;

int m,n;   // �g�c�j�p
int original_maze[50][50];   // ��l�g�c���}�C
int maze[50][50];   // �g�c�ѵ����}�C
int sign[50][50];   // �g�c�g�L���}�C
int top=-1;
int p,q;   // �{�b���쪺��m

int top_move=-1;   // �ʵe�����|


int ** generate_maze;



TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

void record()   // �k�s���L����m
{
	for(int x=0;x<m;x++)
	{
		for(int y=0;y<n;y++)
		{
			sign[x][y]=original_maze[x][y];
            maze[x][y]=original_maze[x][y];
        }
    }
}

struct stack_com   // ���|�}�C�A�������T��m������
{
	int column,row,orientation;
};
stack_com stack[1500];

struct act   // animation�����|
{
	int v,r;
};
act moving[1500][2];

void clean_stack()   // �n�M�Ű��|�������e�I�I�I
{
	while(top!=-1)
	{
		stack[top].column=0;
		stack[top].row=0;
		stack[top--].orientation=0;
	}
	while(top_move!=-1)
	{
		moving[top_move][1].v=0;
		moving[top_move][1].r=0;
		moving[top_move][2].v=0;
		moving[top_move--][2].r=0;
	}
}

struct direction   // ���ʪ���V
{
	int x,y;
};
direction dir[4];

void push(int a,int b,int c)
{
	stack[++top].column=a;   // ����++top�T���A�|��top=top+3
	stack[top].row=b;
	stack[top].orientation=c;
	Form1->Memo1->Lines->Add("Push: "+IntToStr(a)+" "+IntToStr(b)+" "+IntToStr(c));
	Form1->Memo1->Refresh();   // debug��
}

void pop()
{
	int temp;   // �L�X��V
	maze[p][q]=0;
	p=stack[top].column;
	q=stack[top].row;
	temp=stack[top].orientation;
    top--;
	Form1->Memo1->Lines->Add("Pop: "+IntToStr(p)+" "+IntToStr(q)+" "+IntToStr(temp));
	Form1->Memo1->Refresh();   // debug��
}

void run()   // main funtion
{
	dir[0].x=0; dir[0].y=1;   // dir[0] -> ���k
	dir[1].x=1; dir[1].y=0;   // dir[1] -> ���U
	dir[2].x=0; dir[2].y=-1;   // dir[2] -> ����
	dir[3].x=-1; dir[3].y=0;   // dir[3] -> ���W

	int count=0;   // �C�B����V
	while(p!=(m-2) || q!=(n-1))   // ����I�e(�פ����while����Arun�~���A�o�˨���I�����|����)
	{
		if(sign[p+dir[count].x][q+dir[count].y]==0)   // ��S���L����
		{
			push(p,q,count);
			p=p+dir[count].x;
			q=q+dir[count].y;
			sign[p][q]=3;   // �����w�g���L
			moving[++top_move][1].v=p;   // animation�����|
			moving[top_move][1].r=q;
			maze[p][q]=3;
			count=0;   // ��V�n�k�s�A�դU������V�~�|�q���V�}�l
		}
		else count++;

		if(count>=4)   // �|�Ӥ�V���椣�q
		{
			moving[++top_move][2].v=p;   // animation�����|
			moving[top_move][2].r=q;
			pop();
			count=0;   // ��V�n�k�s�A�դU������V�~�|�q���V�}�l
		}
	}

}


// �H�U�� Generate Maze
int ** generate(int ** generate_maze,int x,int y)
{
	int direction;
	generate_maze[x][y] =0; // �Хܦ���w�]�w
   // Form1->Memo2->Lines->Add("(x,y,dir)="+IntToStr(x)+","+IntToStr(y)+","+IntToStr(direction));
	while((y+2<n && generate_maze[x][y+2]==1) || (x+2<m && generate_maze[x+2][y]==1) || (y-2>0 && generate_maze[x][y-2]==1) || (x-2>0 && generate_maze[x-2][y]==1)) // �p�G���O�~��
	{
		direction = rand()%4+1; // �M�w�U�@�Ӧ�m
		if(direction==1 && y+2 < n && generate_maze[x][y+2]==1) // �V�k��
		{
			generate_maze[x][y+1] =0; // ��k��
			generate_maze = generate(generate_maze, x,y+2);
		}
		else if(direction==2 && x-2>0 && generate_maze[x-2][y]==1) // �V�W��
		{
			generate_maze[x-1][y] =0; // ��W��
			generate_maze = generate(generate_maze, x-2,y);
		}
		else if(direction==3 && y-2 > 0 && generate_maze[x][y-2]==1) // �V����
		{
			generate_maze[x][y-1] =0; // ��k��
			generate_maze = generate(generate_maze, x,y-2);
		}
		else if(direction==4 && x+2 < m && generate_maze[x+2][y]==1) // �V�U��
		{
			generate_maze[x+1][y] =0; // ��W��
			generate_maze = generate(generate_maze, x+2,y);
		}
	}
	return generate_maze;
}


//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject *Sender)
{
	// Load Maze
	StringGrid1->Visible=1;   // ���g�c�A���StringGrid
	StringGrid2->Visible=1;
	StringGrid3->Visible=1;

	FILE *fp; // Define fp as a pointer pointing to some file (with data type FILE) in HD
	String out;
	AnsiString infile;   // Use builder's string (AnsiString) to ease the conversion (into const char *)
	if(OpenDialog1->Execute())
	{
		infile = OpenDialog1->FileName;
		fp = fopen(infile.c_str(), "r+");   //c_str��?���Ǧ^�ȬO const char *, �Y��AnsiString�নconst char *��fopen�ϥ�
		fscanf(fp, "%d %d", &m, &n);    // Read in two integers m & n
		Memo1->Lines->Add("Column�G"+IntToStr(m)+"   Row�G"+IntToStr(n));
		Memo1->Lines->Add("---------- Original Maze ----------");
		for(int x=0;x<m;x++)   // Reda in m*n 0/1/2's into maze[][]
		{
			for(int y=0;y<n;y++)
			{
				fscanf(fp, "%d", &original_maze[x][y]);
            }
		}
		fclose(fp);

		for(int x=0;x<m;x++)      // Print out original_maze[][] in Memo1
		{
			out = "";
			for (int y=0;y<n;y++) out += "  "+IntToStr(original_maze[x][y]);
			Memo1->Lines->Add(out);
		}

		StringGrid1->RowCount=m;   // Print out original_maze[][] in StringGrid1
		StringGrid1->ColCount=n;
		StringGrid2->RowCount=m;   // Print out original_maze[][] in StringGrid2
		StringGrid2->ColCount=n;
		StringGrid3->RowCount=m;   // Print out original_maze[][] in StringGrid3
		StringGrid3->ColCount=n;
		for(int x=0;x<m;x++)
			for(int y=0;y<n;y++)
			{
				StringGrid1->Cells[y][x] = original_maze[x][y];StringGrid1->Cells[y][x] = original_maze[x][y];
				StringGrid2->Cells[y][x] = original_maze[x][y];StringGrid1->Cells[y][x] = original_maze[x][y];
                StringGrid3->Cells[y][x] = original_maze[x][y];StringGrid1->Cells[y][x] = original_maze[x][y];
			}
	}


    int grid_size = Edit3->Text.ToInt();   // StringGrid��l���j�p
	for(int i=0; i<m; i++) StringGrid1->RowHeights[i] = grid_size;
	for(int i=0; i<n; i++) StringGrid1->ColWidths[i] = grid_size;
	for(int i=0; i<m; i++) StringGrid2->RowHeights[i] = grid_size;
	for(int i=0; i<n; i++) StringGrid2->ColWidths[i] = grid_size;
	for(int i=0; i<m; i++) StringGrid3->RowHeights[i] = grid_size;
	for(int i=0; i<n; i++) StringGrid3->ColWidths[i] = grid_size;
	StringGrid1->Refresh();
	StringGrid2->Refresh();
	StringGrid3->Refresh();

	Button4->Visible=1;   // ���Ͱg�c�����Button"Find A Tour"
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	// Generate Maze
	if(Edit1->Text.ToInt()>50 || Edit2->Text.ToInt()>50)   // �T�O��J�g�c�j�p���|�L�j�A�y������L�k�ާ@
	{
		ShowMessage("The Maze Should Be Smaller Than 50*50");
	}
	else
	{
		StringGrid1->Visible=1;   // ���g�c�A���StringGrid
		StringGrid2->Visible=1;
		StringGrid3->Visible=1;

		int grid_size = Edit3->Text.ToInt();   // StringGrid��l���j�p
		for(int i=0; i<m; i++) StringGrid1->RowHeights[i] = grid_size;
		for(int i=0; i<n; i++) StringGrid1->ColWidths[i] = grid_size;
		for(int i=0; i<m; i++) StringGrid2->RowHeights[i] = grid_size;
		for(int i=0; i<n; i++) StringGrid2->ColWidths[i] = grid_size;
		for(int i=0; i<m; i++) StringGrid3->RowHeights[i] = grid_size;
		for(int i=0; i<n; i++) StringGrid3->ColWidths[i] = grid_size;
		StringGrid1->Refresh();
		StringGrid2->Refresh();
		StringGrid3->Refresh();

		int ** new_maze;
		m=Edit1->Text.ToInt();
		n=Edit2->Text.ToInt();
		new_maze = new int * [m];
		for (int a=0; a<m; a++) new_maze[a] = new int [n];
		if (new_maze) // delete the previous new_maze
		{
			for (int i=0; i<m; i++) delete(new_maze[i]);
			delete(new_maze);
		}
		int Start_i=1, Start_j=1;
		int End_i=m-2, End_j=n-2;
		// srand(time(NULL));
		for (int x=0; x<m; x++)
		{
			for (int y=0; y<n; y++)
			{
				if (x==0 || y==0 || x==m-1 || y==n-1 ) new_maze[x][y] = 2; // �]�w�~��
				else new_maze[x][y] = 1; // ��l�g�c����
			}
		}
		new_maze = generate(new_maze, End_i, End_j); // ���Ͱg�c
		new_maze[Start_i][Start_j-1] =0; // ��J�f����
		new_maze[End_i][End_j+1] =0; // ��X�f�k��

		for(int a=0;a<m;a++)
		{
			for(int b=0;b<n;b++) original_maze[a][b]=new_maze[a][b];
		}
		original_maze[1][0]=3;   // �_�I�]�n�⨫�L
		original_maze[1][1]=0;   // ���ƪ�maze�X�f�|�Q��צ�
		if(m%2==0 && n%2==0) original_maze[2][1]=0;   // �Y��C���������٭n�h�}�@�����

		StringGrid1->RowCount = m;
		StringGrid1->ColCount = n;
		StringGrid2->RowCount = m;
		StringGrid2->ColCount = n;
		StringGrid3->RowCount = m;
		StringGrid3->ColCount = n;
		for (int i=0; i<m; i++)
			for (int j=0; j<n; j++)
			{
				StringGrid1->Cells[j][i] = original_maze[i][j];
				StringGrid2->Cells[j][i] = original_maze[i][j];
				StringGrid3->Cells[j][i] = original_maze[i][j];
			}

		Button4->Visible=1;   // ���Ͱg�c�����Button"Find A Tour"
	}
}

//---------------------------------------------------------------------------



void __fastcall TForm1::Button4Click(TObject *Sender)
{
	// Find a Tour
	p=1; q=0;   // �q(1,0)�}�l��
	top=-1;     // top�k�s(-1)
	record();   // �k�s���L����m
	clean_stack();   // �n�M�Ű��|�������e�I�I�I
	run();   // �}�l���g�c
	StringGrid1->RowCount=m;   // Print out sign[][] in StringGrid1
	StringGrid1->ColCount=n;
	StringGrid2->RowCount=m;   // Print out original_maze[][] in StringGrid1
	StringGrid2->ColCount=n;

	String out;
	Memo1->Lines->Add("---------- Final Maze ----------");
	for(int x=0;x<m;x++)
	{
		out = "";
		for(int y=0;y<n;y++)
		{
			out += "  "+IntToStr(maze[x][y]);
		}
		Memo1->Lines->Add(out);
	}

	for(int x=0;x<m;x++)   // �L�X���㨫�����g�c
	{
		for(int y=0;y<n;y++) StringGrid2->Cells[y][x] = maze[x][y];
	}

    for(int x=0;x<m;x++)   // �L�X���L�����|
	{
		for(int y=0;y<n;y++) StringGrid1->Cells[y][x] = sign[x][y];
	}

	Button6->Visible=1;   // �]���g�c�����Button"Cool Animation"
}
//---------------------------------------------------------------------------


void __fastcall TForm1::StringGrid2DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
	String text = StringGrid2->Cells[ACol][ARow];
	if (text == "0") StringGrid2->Canvas->Brush->Color = TColor RGB(239, 255, 253);
	else if (text == "1") StringGrid2->Canvas->Brush->Color = TColor (RGB(184, 255, 249));
	else if (text == "2") StringGrid2->Canvas->Brush->Color = TColor (RGB(133, 244, 255));
	else if (text == "3") StringGrid2->Canvas->Brush->Color = TColor (RGB(66, 194, 255));
	else StringGrid2->Canvas->Brush->Color = TColor (RGB(255, 247, 188));
	StringGrid2->Canvas->FillRect(Rect);
	// StringGrid2->Canvas->TextRect(Rect, Rect.Left+6, Rect.Top+8, text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid3DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
	String text = StringGrid3->Cells[ACol][ARow];
	if (text == "0") StringGrid3->Canvas->Brush->Color = TColor RGB(239, 255, 253);
	else if (text == "1") StringGrid3->Canvas->Brush->Color = TColor (RGB(184, 255, 249));
	else if (text == "2") StringGrid3->Canvas->Brush->Color = TColor (RGB(133, 244, 255));
	else if (text == "3") StringGrid3->Canvas->Brush->Color = TColor (RGB(66, 194, 255));
	else StringGrid3->Canvas->Brush->Color = TColor (RGB(255, 247, 188));
	StringGrid3->Canvas->FillRect(Rect);
	// StringGrid3->Canvas->TextRect(Rect, Rect.Left+6, Rect.Top+8, text);
}
//---------------------------------------------------------------------------


void __fastcall TForm1::StringGrid1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
	String text = StringGrid1->Cells[ACol][ARow];
	if (text == "0") StringGrid1->Canvas->Brush->Color = TColor RGB(239, 255, 253);
	else if (text == "1") StringGrid1->Canvas->Brush->Color = TColor (RGB(184, 255, 249));
	else if (text == "2") StringGrid1->Canvas->Brush->Color = TColor (RGB(133, 244, 255));
	else if (text == "3") StringGrid1->Canvas->Brush->Color = TColor (RGB(66, 194, 255));
	else StringGrid1->Canvas->Brush->Color = TColor (RGB(255, 247, 188));
	StringGrid1->Canvas->FillRect(Rect);
	// StringGrid1->Canvas->TextRect(Rect, Rect.Left+6, Rect.Top+8, text);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Form1->Memo1->Lines->Clear();   // �M��Memo�̭������e
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
	// Cool Animation
	int a,b;
	for(int x=0;x<m;x++)   // ���s�}�l�e�M�ŤW�@���]�L������
	{
		for(int y=0;y<n;y++)
		{
			if(StringGrid3->Cells[y][x]==3 || StringGrid3->Cells[y][x]==4)
			{
				StringGrid3->Cells[y][x]=0;
            }
		}
	}

    StringGrid3->Cells[0][1]=3;   // �]��stack�|��Ĥ@����1�A�ҥH���]�w��3�A�]�_���C������[
	for(int i=0;i<=top_move;i++)
	{
		Sleep(TrackBar1->Position*100);
		if(moving[i][1].v != 0)
		{
			a=moving[i][1].v;
			b=moving[i][1].r;
			StringGrid3->Cells[b][a]=3;
			StringGrid3->Refresh();
		}
		else if(moving[i][2].v != 0)
		{
			a=moving[i][2].v;
			b=moving[i][2].r;
			StringGrid3->Cells[b][a]=4;
			StringGrid3->Refresh();
		}
	}
	StringGrid3->Cells[n-1][m-2]=3;   // �ѩ�stack����I�N�ߨ谱��F�A���F�]�_���C������[�A�]�w��3
	StringGrid3->Refresh();
    ShowMessage("�Y�n�A�ݤ@���ʵe�A�ЦA���@��Find a Tour");
	clean_stack();   // �n�M�Ű��|�������e�I�I�I

}
//---------------------------------------------------------------------------



void __fastcall TForm1::Button5Click(TObject *Sender)
{
    // Play A Game
	StringGrid4->Visible=1;
    m=Edit1->Text.ToInt();
	n=Edit2->Text.ToInt();
	p=m-2; q=n-1;   // ���F�i�Jwhile�j��

	// Generate Maze
	p=1; q=0;
		if(Edit1->Text.ToInt()>50 || Edit2->Text.ToInt()>50)
		{
			ShowMessage("The Maze Should Be Smaller Than 50*50");
		}
		else
		{
			int grid_size = Edit3->Text.ToInt();
			for(int i=0; i<m; i++) StringGrid4->RowHeights[i] = grid_size;
			for(int i=0; i<n; i++) StringGrid4->ColWidths[i] = grid_size;
			StringGrid4->Refresh();

			int ** new_maze;
			new_maze = new int * [m];
			for (int a=0; a<m; a++) new_maze[a] = new int [n];
			if (new_maze)
			{
				for (int i=0; i<m; i++) delete(new_maze[i]);
				delete(new_maze);
			}
			int Start_i=1, Start_j=1;
			int End_i=m-2, End_j=n-2;
			// srand(time(NULL));
			for (int x=0; x<m; x++)
			{
				for (int y=0; y<n; y++)
				{
					if (x==0 || y==0 || x==m-1 || y==n-1 ) new_maze[x][y] = 2; // �]�w�~��
					else new_maze[x][y] = 1; // ��l�g�c����
				}
			}
			new_maze = generate(new_maze, End_i, End_j); // ���Ͱg�c
			new_maze[Start_i][Start_j-1] =0; // ��J�f����
			new_maze[End_i][End_j+1] =0; // ��X�f�k��

			for(int a=0;a<m;a++)
			{
				for(int b=0;b<n;b++) original_maze[a][b]=new_maze[a][b];
			}
			original_maze[1][0]=3;   // ���[
			original_maze[1][1]=0;   // ���ƪ�maze�X�f�|�Q��צ�
			if(m%2==0 && n%2==0) original_maze[2][1]=0;   // �Y��C���������٭n�h�}�@�����

			StringGrid4->RowCount = m;
			StringGrid4->ColCount = n;
			for (int i=0; i<m; i++)
				for (int j=0; j<n; j++)
				{
					StringGrid4->Cells[j][i] = original_maze[i][j];
				}
		}

}
//---------------------------------------------------------------------------


void __fastcall TForm1::StringGrid4KeyPress(TObject *Sender, System::WideChar &Key)

{
	switch(Key)
	{
		case 'd':
		{
			if(original_maze[p][q+1]==0)
			{
				original_maze[p][q]=0;
				StringGrid4->Cells[q][p] = original_maze[p][q];
				q++;
				original_maze[p][q]=3;
				StringGrid4->Cells[q][p] = original_maze[p][q];
			}

			if(p==m-2 && q==n-1)   // ����I�n���s���g�c���s
			{
            	p=1; q=0;
				int ** new_maze;
				new_maze = new int * [m];
				for (int a=0; a<m; a++) new_maze[a] = new int [n];
                for (int a=0; a<m; a++) new_maze[a] = new int [n];
				if (new_maze)
				{
					for (int i=0; i<m; i++) delete(new_maze[i]);
					delete(new_maze);
				}
				int Start_i=1, Start_j=1;
				int End_i=m-2, End_j=n-2;
				// srand(time(NULL));
				for (int x=0; x<m; x++)
				{
					for (int y=0; y<n; y++)
					{
						if (x==0 || y==0 || x==m-1 || y==n-1 ) new_maze[x][y] = 2; // �]�w�~��
						else new_maze[x][y] = 1; // ��l�g�c����
					}
				}
				new_maze = generate(new_maze, End_i, End_j); // ���Ͱg�c
				new_maze[Start_i][Start_j-1] =0; // ��J�f����
				new_maze[End_i][End_j+1] =0; // ��X�f�k��

				for(int a=0;a<m;a++)
				{
					for(int b=0;b<n;b++) original_maze[a][b]=new_maze[a][b];
				}
				original_maze[1][0]=3;   // ���[
				original_maze[1][1]=0;   // ���ƪ�maze�X�f�|�Q��צ�
				if(m%2==0 && n%2==0) original_maze[2][1]=0;   // �Y��C���������٭n�h�}�@�����
				for(int a=0;a<m;a++)
				{
					for(int b=0;b<n;b++) StringGrid4->Cells[b][a] = original_maze[a][b];
				}
				StringGrid4->Refresh();
			}
		}
		break;
		case 's':
			if(original_maze[p+1][q]==0)
			{
				original_maze[p][q]=0;
				StringGrid4->Cells[q][p] = original_maze[p][q];
				p++;
				original_maze[p][q]=3;
				StringGrid4->Cells[q][p] = original_maze[p][q];
			}
			StringGrid4->Refresh();
			break;
		case 'a':
			if(p!=1 || q!=0)
			{
                if(original_maze[p][q-1]==0)
				{
					original_maze[p][q]=0;
					StringGrid4->Cells[q][p] = original_maze[p][q];
					q--;
					original_maze[p][q]=3;
					StringGrid4->Cells[q][p] = original_maze[p][q];
				}
				StringGrid4->Refresh();
			}
            else ShowMessage("You cannot go back�I");
			break;
		case  'w':
			if(original_maze[p-1][q]==0)
            {
				original_maze[p][q]=0;
                StringGrid4->Cells[q][p] = original_maze[p][q];
				p--;
				original_maze[p][q]=3;
                StringGrid4->Cells[q][p] = original_maze[p][q];
			}
			StringGrid4->Refresh();
			break;
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::StringGrid4DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
	String text = StringGrid4->Cells[ACol][ARow];
	if (text == "0") StringGrid4->Canvas->Brush->Color = TColor RGB(239, 255, 253);
	else if (text == "1") StringGrid4->Canvas->Brush->Color = TColor (RGB(184, 255, 249));
	else if (text == "2") StringGrid4->Canvas->Brush->Color = TColor (RGB(133, 244, 255));
	else if (text == "3") StringGrid4->Canvas->Brush->Color = TColor (RGB(77, 119, 255));
	else StringGrid4->Canvas->Brush->Color = TColor (RGB(255, 247, 188));
	StringGrid4->Canvas->FillRect(Rect);
	// StringGrid4->Canvas->TextRect(Rect, Rect.Left+6, Rect.Top+8, text);
}
//---------------------------------------------------------------------------



