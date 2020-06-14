unit MainformRandPuzzles;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,Math,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.Grids, Vcl.StdCtrls, Vcl.Samples.Spin, VclTee.TeeGDIPlus, VCLTee.TeEngine,
  VCLTee.Series, Vcl.ExtCtrls, VCLTee.TeeProcs, VCLTee.Chart;

type

   TPieces = record
      T,R,B,L        : Byte;
      aRow,aCol      : Byte;              // col and row indicies of where this piece fits in the solution
   end;

  TForm1 = class(TForm)
    SpinEdit1: TSpinEdit;
    Label1: TLabel;
    Button1: TButton;
    Label2: TLabel;
    SpinEdit2: TSpinEdit;
    Label3: TLabel;
    SpinEdit3: TSpinEdit;
    Button2: TButton;
    Button3: TButton;
    Button4: TButton;
    Button5: TButton;
    GroupBox1: TGroupBox;
    Button6: TButton;
    SpinEdit4: TSpinEdit;
    Label4: TLabel;
    CheckBox2: TCheckBox;
    GroupBox2: TGroupBox;
    Label5: TLabel;
    Button7: TButton;
    SpinEdit5: TSpinEdit;
    CheckBox4: TCheckBox;
    Memo1: TMemo;
    Label6: TLabel;
    Chart1: TChart;
    Series1: TBarSeries;
    procedure SpinEdit1Change(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure Button3Click(Sender: TObject);
    procedure Button4Click(Sender: TObject);
    procedure Button5Click(Sender: TObject);
    procedure Button6Click(Sender: TObject);
    procedure SpinEdit2Change(Sender: TObject);
    procedure SpinEdit3Change(Sender: TObject);
    procedure Button7Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;
  PuzzleSize      : Integer;
  nBorderSides,nInnerSides         : Integer;
  nBorderColours,nInnerColours         : Integer;
  nCorners,nEdges,nInners  : Integer;
  BorderColour    : Array[0..100] of Byte;
  InsideColour    : Array[0..800] of Byte;
  ColourID        : Array[1..30] of Byte;
  nTiles          : Integer;
  Piece           : Array[1..16,1..16] of TPieces;           // indicies are col,row   -  that is, (x,y) co-ordinates rather than matrix indicies
  PieceID         : Array[1..16,1..16] of Integer;
  OriginalPiece   : Array[1..257] of TPieces;
  nDup,nSym       : Integer;
  fName           : String;

//
//                   Border colours must scale as  5 - 16x16
//                   Inside colours must scale as 18 - 16x16
//
//                   This will match the ratio of the Eternity II puzzle distribution
//

implementation

{$R *.dfm}

function RotatePiece(i,j,r: integer): TPieces;                  // rotates r*90 degrees clockwise
var                                                                  // but the original piece ID does not change!
   k,n:integer;
begin
   result:=Piece[i,j];
   for n:=1 to r do with result do
   begin
      k:=T;T:=L;L:=B;B:=R;R:=k;
   end;
end;

function Rotate(i,r: integer): TPieces;                  // rotates r*90 degrees clockwise
var                                                                  // but the original piece ID does not change!
   k,n:integer;
begin
   result:=OriginalPiece[i];
   for n:=1 to r do with result do
   begin
      k:=T;T:=L;L:=B;B:=R;R:=k;
   end;
end;

function SamePiece(a,b:TPieces):Boolean;
begin
   Result:=(a.T=b.T) and (a.R=b.R) and (a.B=b.B) and (a.L=b.L);
end;

procedure TForm1.Button2Click(Sender: TObject);
var
   i,j:Integer;
begin
   nSym:=0;                                                    // initialize count of number of symmetric pieces
   for i:=1 to PuzzleSize do for j:=1 to PuzzleSize do
   with Piece[i,j] do if (T=B) and (L=R) then inc(nSym);       // only symmetric if Top pattern = Bot pattern, and L = R
end;

procedure TForm1.Button3Click(Sender: TObject);
var
   i,j,n:Integer;
begin
   for i:=1 to PuzzleSize-1 do Piece[PuzzleSize,i]:=RotatePiece(PuzzleSize,i,3);                             // rotate the RHS border back
   for i:=1 to PuzzleSize-1 do Piece[PuzzleSize-i+1,PuzzleSize]:=RotatePiece(PuzzleSize-i+1,PuzzleSize,2);   // rotate the BOT border back
   for i:=1 to PuzzleSize-1 do Piece[1,PuzzleSize-i+1]:=RotatePiece(1,PuzzleSize-i+1,1);                     // rotate teh LHS border back
   OriginalPiece[1]:=Piece[1,1];                                                         // first the corner pieces
   OriginalPiece[2]:=Piece[PuzzleSize,1];
   OriginalPiece[3]:=Piece[1,PuzzleSize];
   OriginalPiece[4]:=Piece[PuzzleSize,PuzzleSize];
   PieceID[1,1]:=1;
   PieceID[PuzzleSize,1]:=2;
   PieceID[1,PuzzleSize]:=3;
   PieceID[PuzzleSize,PuzzleSize]:=4;
   n:=4;                                                                                 // now the edge pieces
   for i:=2 to PuzzleSize-1 do
   begin
      inc(n);
      OriginalPiece[n]:=Piece[i,1];PieceID[i,1]:=n;
   end;
   for i:=2 to PuzzleSize-1 do
   begin
      inc(n);
      OriginalPiece[n]:=Piece[PuzzleSize,i];PieceID[PuzzleSize,i]:=n;
   end;
   for i:=2 to PuzzleSize-1 do
   begin
      inc(n);
      OriginalPiece[n]:=Piece[PuzzleSize-i+1,PuzzleSize];PieceID[PuzzleSize-i+1,PuzzleSize]:=n;
   end;
   for i:=2 to PuzzleSize-1 do
   begin
      inc(n);
      OriginalPiece[n]:=Piece[1,PuzzleSize-i+1];PieceID[1,PuzzleSize-i+1]:=n;
   end;
   for i:=2 to PuzzleSize-1 do for j:=2 to PuzzleSize-1 do                               // finally the inner pieces
   begin
      inc(n);
      OriginalPiece[n]:=Piece[j,i];PieceID[j,i]:=n;
   end;
   nDup:=0;
   for i:=1 to 3 do                                                   // test corners first
   for j:=i+1 to 4 do
   if SamePiece(OriginalPiece[i],OriginalPiece[j]) then inc(nDup);
   for i:=5 to nEdges+3 do                                            // now the edges
   for j:=i+1 to nEdges+4 do
   if SamePiece(OriginalPiece[i],OriginalPiece[j]) then inc(nDup);
   for i:=nEdges+5 to nTiles-1 do                                     // finally the inner pieces
   for j:=i+1 to nTiles do
   begin
      if SamePiece(OriginalPiece[i],OriginalPiece[j]) then inc(nDup) else
      if SamePiece(OriginalPiece[i],Rotate(j,1)) then inc(nDup) else
      if SamePiece(OriginalPiece[i],Rotate(j,2)) then inc(nDup) else
      if SamePiece(OriginalPiece[i],Rotate(j,3)) then inc(nDup)
   end;
end;

procedure TForm1.Button4Click(Sender: TObject);
var
   i,n:integer;
begin
   n:=0;
   repeat
      inc(n);
      Button1Click(nil);
      Button2Click(nil);
      Button3Click(nil);
   until (nSym=0) and (nDup=0);
   if Sender<>Nil then
   begin
      with Chart1.Series[0] do
      begin
         Clear;
         for i:=1 to nBorderColours do AddXY(i,ColourID[i],' ',clGreen);
         for i:=nBorderColours+1 to nBorderColours+nInnerColours do AddXY(i,ColourID[i],' ',clBlue);
      end;
      Memo1.Lines.Append('That took '+intToStr(n)+' random attempts to get 1 fair Eternity II puzzle');
   end;
end;

procedure TForm1.Button5Click(Sender: TObject);
var
   f:TextFile;
   i,j,k:Integer;

   function FindPieceID(j,i:Byte):Byte;
   begin
      Result:=1;
      while (Result<nTiles) and ((OriginalPiece[Result].aRow<>i) or (OriginalPiece[Result].aCol<>j)) do inc(Result);
   end;
begin
   if fName='' then fName:='Output.txt';
   assignFile(f,fname);
   rewrite(f);
   writeln(f,PuzzleSize:2,'':10,'puzzle size (rows and columns)');
   writeln(f,nBorderColours:2,nInnerColours:4,'':6,'# border colours  # inside colours');
   writeln(f,'Top Rgt Bot Lft');
   if not CheckBox4.Checked then for i:=1 to nTiles do with OriginalPiece[i] do writeln(f,T:2,R:3,B:3,L:3) else
   begin
      for i:=1 to 3 do
      begin
         j:=Random(4)+1;
         repeat
            k:=Random(4)+1;
         until k<>j;
         OriginalPiece[nTiles+1]:=OriginalPiece[j];
         OriginalPiece[j]:=OriginalPiece[k];
         OriginalPiece[k]:=OriginalPiece[nTiles+1];
      end;
      for i:=1 to nEdges do
      begin
         j:=Random(nEdges)+5;
         repeat
            k:=Random(nEdges)+5;
         until k<>j;
         OriginalPiece[nTiles+1]:=OriginalPiece[j];
         OriginalPiece[j]:=OriginalPiece[k];
         OriginalPiece[k]:=OriginalPiece[nTiles+1];
      end;
      for i:=1 to nInners do
      begin
         j:=Random(nInners)+nEdges+5;
         repeat
            k:=Random(nInners)+nEdges+5;
         until k<>j;
         OriginalPiece[nTiles+1]:=OriginalPiece[j];
         OriginalPiece[j]:=OriginalPiece[k];
         OriginalPiece[k]:=OriginalPiece[nTiles+1];
      end;
      for i:=1 to nTiles do with OriginalPiece[i] do writeln(f,T:2,R:3,B:3,L:3);
   end;
   if CheckBox2.Checked then
   begin
      Writeln(f,'2x2 solution sets');
                                                                                          // corner pieces first
      Writeln(f,'  0 0 3 0',FindPieceID(1,1):4,FindPieceID(2,1):4,FindPieceID(1,2):4,FindPieceID(2,2):4);
      Writeln(f,'  0 1 0 1',FindPieceID(PuzzleSize-1,1):4,FindPieceID(PuzzleSize,1):4,FindPieceID(PuzzleSize-1,2):4,FindPieceID(PuzzleSize,2):4);
      Writeln(f,'  3 0 3 2',FindPieceID(1,PuzzleSize-1):4,FindPieceID(2,PuzzleSize-1):4,FindPieceID(1,PuzzleSize):4,FindPieceID(2,PuzzleSize):4);
      Writeln(f,'  0 1 2 2',FindPieceID(PuzzleSize-1,PuzzleSize-1):4,FindPieceID(PuzzleSize,PuzzleSize-1):4,FindPieceID(PuzzleSize-1,PuzzleSize):4,FindPieceID(PuzzleSize,PuzzleSize):4);

                                                                                          // edge pieces next
      for i:=2 to (PuzzleSize div 2)-1 do Writeln(f,'  0 0 0 0',FindPieceID(2*i-1,1):4,FindPieceID(2*i,1):4,FindPieceID(2*i-1,2):4,FindPieceID(2*i,2):4);
      for i:=2 to (PuzzleSize div 2)-1 do Writeln(f,'  0 1 0 1',FindPieceID(PuzzleSize-1,2*i-1):4,FindPieceID(PuzzleSize,2*i-1):4,FindPieceID(PuzzleSize-1,2*i):4,FindPieceID(PuzzleSize,2*i):4);
      for i:=2 to (PuzzleSize div 2)-1 do Writeln(f,'  0 0 2 2',FindPieceID(2*i-1,PuzzleSize-1):4,FindPieceID(2*i,PuzzleSize-1):4,FindPieceID(2*i-1,PuzzleSize):4,FindPieceID(2*i,PuzzleSize):4);
      for i:=2 to (PuzzleSize div 2)-1 do Writeln(f,'  3 0 3 0',FindPieceID(1,2*i-1):4,FindPieceID(2,2*i-1):4,FindPieceID(1,2*i):4,FindPieceID(2,2*i):4);

                                                                                          // now the easy bits - the inner pieces
      for i:=2 to (PuzzleSize div 2)-1 do for j:=2 to (PuzzleSize div 2)-1 do Writeln(f,'  0 0 0 0',FindPieceID(2*j-1,2*i-1):4,FindPieceID(2*j,2*i-1):4,FindPieceID(2*j-1,2*i):4,FindPieceID(2*j,2*i):4, ' *');
   end;
   Writeln(f,'One single tile solution to this puzzle - there may be more');
   for i:=1 to PuzzleSize do
   begin
      for j:=1 to PuzzleSize do write(f,FindPieceID(j,i):4);
      writeln(f);
   end;
   closeFile(f);
   if Sender<>Nil then Memo1.Lines.Append('Tiles saved to file '+fName);
end;

procedure TForm1.Button6Click(Sender: TObject);
var
   i,j,k,n:Integer;
begin
   n:=spinEdit4.value;
   for i:=1 to n do
   begin
      for j:=2 to 8 do
      begin
         k:=j*2;
         SpinEdit1.Value:=k;
         Button4Click(nil);
         if k<10 then fName:='RandPuzzle0'+IntTostr(k)+'x0'+IntTostr(k)+'_'+IntToStr(i+10)+'.txt'
                 else fName:='RandPuzzle'+IntTostr(k)+'x'+IntTostr(k)+'_'+IntToStr(i+10)+'.txt';
         Button5Click(nil);
      end;
   end;
end;

procedure TForm1.Button7Click(Sender: TObject);
var
   i,k,n:Integer;
begin
   n:=spinEdit5.value;
   for i:=1 to n do
   begin
      k:=SpinEdit1.Value;
      Button4Click(nil);
      if i<10 then fName:='RandPuzzle'+IntTostr(k)+'x'+IntTostr(k)+'_0'+IntToStr(i)+'.txt' else fName:='RandPuzzle'+IntTostr(k)+'x'+IntTostr(k)+'_'+IntToStr(i)+'.txt';
      Button5Click(nil);
      Memo1.Lines.Append('File #'+IntToStr(i)+fName+' generated and saved in the same dir as the exe');
   end;
   Memo1.lines.append('-------------All files completed-----------------');
end;

procedure TForm1.FormShow(Sender: TObject);
begin
   SpinEdit1Change(Nil);
end;

procedure TForm1.SpinEdit1Change(Sender: TObject);
begin
   PuzzleSize:=SpinEdit1.Value;
   nTiles:=sqr(PuzzleSize);
   nCorners:=4;
   nEdges:=4*(PuzzleSize-2);
   nInners:=nTiles-(nCorners+nEdges);
   nBorderSides:=4*(PuzzleSize-1);
   nInnerSides:=2*(PuzzleSize-1)*(PuzzleSize-2);
   case PuzzleSize of
      4 : begin nBorderColours:=3;nInnerColours:= 4 end;
      6 : begin nBorderColours:=3;nInnerColours:= 6 end;
      8 : begin nBorderColours:=3;nInnerColours:= 8 end;
      10: begin nBorderColours:=4;nInnerColours:=10 end;
      12: begin nBorderColours:=4;nInnerColours:=12 end;
      14: begin nBorderColours:=5;nInnerColours:=15 end;
      16: begin nBorderColours:=5;nInnerColours:=17 end;
   end;
   SpinEdit2.Value:=nBorderColours;
   SpinEdit3.Value:=nInnerColours;
end;

procedure TForm1.SpinEdit2Change(Sender: TObject);
begin
   nBorderColours:=SpinEdit2.Value;
end;

procedure TForm1.SpinEdit3Change(Sender: TObject);
begin
   nInnerColours:=SpinEdit3.Value;
end;

procedure TForm1.Button1Click(Sender: TObject);
var
   i,j,n:Integer;

   procedure SwapBorders(a,b:Integer);             // swaps border colours in positions a and b from the border colour list
   var
      i:Byte;
   begin
      i:=BorderColour[a];BorderColour[a]:=BorderColour[b];BorderColour[b]:=i;
   end;

   procedure SwapInsides(a,b:Integer);             // swaps inner  colours in positions a and b from the border colour list
   var
      i:Byte;
   begin
      i:=InsideColour[a];InsideColour[a]:=InsideColour[b];InsideColour[b]:=i;
   end;

begin
   for i:=1 to 30 do ColourID[i]:=0;
   for i:=1 to nBorderSides do                                                                     // make a list of colours to be assigned to border tile edges (sides)
   begin
      BorderColour[i]:=(i mod nBorderColours) + 1;
      inc(ColourID[BorderColour[i]]);
   end;
   for i:=1 to nInnerSides do                                                                     // make a list of colours to be assigned to inner  tile edges (sides)
   begin
      InsideColour[i]:=(i mod nInnerColours) + 1 + nBorderColours;
      inc(ColourID[InsideColour[i]]);
   end;

   if nBorderColours=2 then                                                                           // if only 2 border colours, then assign manually (there's only 1 way to do it effectively)
   begin
      BorderColour[1]:=1;BorderColour[2]:=1;           // set up the 4 corners first - no degrees of freedom with only 2 colours
      BorderColour[3]:=1;BorderColour[4]:=2;
      BorderColour[5]:=2;BorderColour[6]:=1;
      BorderColour[7]:=2;BorderColour[8]:=2;                                                          // now swap the colours of the edge pieces sides
      for i:=1 to nBorderSides do SwapBorders(Random(nBorderSides-8)+9,Random(nBorderSides-8)+9);
   end else
   begin
      for i:=1 to 2*nBorderSides do SwapBorders(Random(nBorderSides)+1,Random(nBorderSides)+1);       // else just randomly shuffle the colours in the border colours list
   end;

   for i:=1 to 4*nInnerSides do SwapInsides(Random(nInnerSides)+1,Random(nInnerSides)+1);    // randomly swap colours from the list for inner sides
   BorderColour[0]:=BorderColour[nBorderSides];                                              // make sure border colours link once we get back to top LH corner

                                  // now comes the fun part - Imagine all the pieces are on the board, but that we have to assign colours to the borders of each piece.
                                  //                          To that end, as we set the colour on the edge of one piece,
                                  //                          that automatically sets the colour on the mirror edge of the adjacent piece
                                  //                          Start by setting the edges of all 4 corner pieces.

   with Piece[1,1] do                               // top LH corner piece - set colours
   begin
      T:=0;L:=0;R:=BorderColour[1];B:=BorderColour[2];
   end;
   with Piece[PuzzleSize,1] do                      // top RH corner piece - set colours
   begin
      T:=0;L:=0;R:=BorderColour[3];B:=BorderColour[4];
   end;
   with Piece[1,PuzzleSize] do                      // bot LH corner piece - set colours
   begin
      T:=0;L:=0;R:=BorderColour[5];B:=BorderColour[6];
   end;
   with Piece[PuzzleSize,PuzzleSize] do             // bot RH corner piece - set colours
   begin
      T:=0;L:=0;R:=BorderColour[7];B:=BorderColour[8];
   end;
                                  //   Now set the colours on the rest of the border pieces (i.e, the edge pieces).
   n:=8;
   for i:=1 to PuzzleSize-3 do          // top border edges  - work left to right
   with Piece[1+i,1] do
   begin
      inc(n);
      T:=0;L:=Piece[i,1].R;R:=BorderColour[n];
   end;
   with Piece[PuzzleSize-1,1] do
   begin
      T:=0;L:=Piece[PuzzleSize-2,1].R;R:=Piece[PuzzleSize,1].B;
   end;
   for i:=1 to PuzzleSize-3 do          // RHS border edges   - work from top to bottom
   with Piece[PuzzleSize,1+i] do
   begin
      inc(n);
      T:=0;L:=Piece[PuzzleSize,i].R;R:=BorderColour[n];
   end;
   with Piece[PuzzleSize,PuzzleSize-1] do
   begin
      T:=0;L:=Piece[PuzzleSize,PuzzleSize-2].R;R:=Piece[PuzzleSize,PuzzleSize].B;
   end;
   for i:=1 to PuzzleSize-3 do          // bottom border edges   - work from right back to left
   with Piece[PuzzleSize-i,PuzzleSize] do
   begin
      inc(n);
      T:=0;L:=Piece[PuzzleSize-i+1,PuzzleSize].R;R:=BorderColour[n];
   end;
   with Piece[2,PuzzleSize] do
   begin
      T:=0;L:=Piece[3,PuzzleSize].R;R:=Piece[1,PuzzleSize].B;
   end;
   for i:=1 to PuzzleSize-3 do          // LHS border edges   - work from bottom to top
   with Piece[1,PuzzleSize-i] do
   begin
      inc(n);
      T:=0;L:=Piece[1,PuzzleSize-i+1].R;R:=BorderColour[n];
   end;
   with Piece[1,2] do                 // link last piece to the first piece!!
   begin
      T:=0;L:=Piece[1,3].R;R:=Piece[1,1].B;
   end;
                // rotate the edges - the above assumed everything was on unwrapped to be a single line from left to right

   for i:=1 to PuzzleSize-1 do Piece[PuzzleSize,i]:=RotatePiece(PuzzleSize,i,1);                             // rotate RHS
   for i:=1 to PuzzleSize-1 do Piece[PuzzleSize-i+1,PuzzleSize]:=RotatePiece(PuzzleSize-i+1,PuzzleSize,2);   // rotate bottom
   for i:=1 to PuzzleSize-1 do Piece[1,PuzzleSize-i+1]:=RotatePiece(1,PuzzleSize-i+1,3);                     // rotate LHS

                                  //   Now do the colours on the inner pieces.

   n:=0;
   for i:=2 to PuzzleSize-1 do
   begin
      inc(n);Piece[i,1].B:=InsideColour[n];            // top border
      inc(n);Piece[1,i].R:=InsideColour[n];            // left border
   end;
   for i:=2 to PuzzleSize-1 do       // col
   for j:=2 to PuzzleSize-1 do       // row
   with Piece[i,j] do
   begin
      T:=Piece[i,j-1].B;L:=Piece[i-1,j].R;
      inc(n);B:=InsideColour[n];
      inc(n);R:=InsideColour[n];
   end;
   for i:=2 to PuzzleSize-1 do
   begin
      Piece[PuzzleSize,i].L:=Piece[PuzzleSize-1,i].R;            // right border
      Piece[i,PuzzleSize].T:=Piece[i,PuzzleSize-1].B;            // bottom border
   end;

   for i:=1 to PuzzleSize do for j:=1 to PuzzleSize do           // map indicies of position of piece in solution
   begin
      Piece[i,j].aRow:=j;Piece[i,j].aCol:=i;
   end;
end;

end.
