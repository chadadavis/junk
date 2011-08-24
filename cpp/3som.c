/**********************************/
/* IN MEMORY OF HJALMAR CEULEMANS */
/* WHO NEVER LIVED TO SEE THE SUN */
/**********************************/
/********* 10 - IV - 2003 *********/
/**********************************/

/* Author : Hugo Ceulemans */
/* First public version : 10-IX-2003 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

OutOfMemory(void *Pointer) {
  if(!Pointer) {
    printf("Out of memory\n");
    exit(0);
}}

long int LongIntByteOrderConvert(long int OldInt) {
  long int NewInt;
  char *Old = (char *)&OldInt;
  char *New = (char *)&NewInt;
  New[0] = Old[3];
  New[1] = Old[2];
  New[2] = Old[1];
  New[3] = Old[0];
  return NewInt;
}

float FloatByteOrderConvert(float OldFloat) {
  float NewFloat;
  char *Old = (char *)&OldFloat;
  char *New = (char *)&NewFloat;
  New[0] = Old[3];
  New[1] = Old[2];
  New[2] = Old[1];
  New[3] = Old[0];
  return NewFloat;
}

float AtomToAtomicNumber(char *Atom) {
  float AtomicNumber;
  switch(*Atom) {
    case 'C' : AtomicNumber = 6;
      break;
    case 'O' : AtomicNumber = 8;
      break;
    case 'N' : AtomicNumber = 7;
      break;
    case 'P' : AtomicNumber = 15;
      break;
    case 'S' : AtomicNumber = 16;
      break;
    default : AtomicNumber = 0;
      break;
  } return AtomicNumber;
}

void ConvolveDericheGaussian(float *PdbDensityValues,
                        float ZD, 
                        float YD,
                        float XD,
                        float Sigma,
                        long int DerivationOrder) {

  double A0, A1, ExpB0, Exp2B0, SinO0, CosO0, Scale0, ScaleA;
  double C0, C1, ExpB1, Exp2B1, SinO1, CosO1, Scale1, ScaleC, Scale;
  register float D1, D2, D3, D4, Np0, Np1, Np2, Np3, Nn0, Nn1, Nn2, Nn3, Nn4;
  long int AxisFlags[5] = {1, 1, 0, 1, 1}; /* axis flag array */
  long int AxisIndex; /* array index for Z axis flag */
  float *Intermediate1, *Intermediate2, *LineStart;
  long int Increment, Dimension;
  register float *L0, *L1, *L2, *L3, *L4;
  register float *I0, *I1, *I2, *I3, *I4;
  long int x, y, z, Index;

  /* Based on : Recursively implementing the Gaussian and its derivatives */
  /* (http://www.inria.fr/rrrt/rr-1893.html) */
  /* The code is in part adapted from work by Gregoire Malandain */
  /* (http://www-sop.inria.fr/epidaure/personnel/malandain/) */

  /* parametrisation */
  switch(DerivationOrder) {
    case 0:
      A0     =    1.68;
      A1     =   3.735;
      Exp2B0 =   1.783; /* initialise as B0 */
      CosO0  =  0.6318; /* initialise as O(mega)0 */
      C0     = -0.6803;
      C1     = -0.2598;
      Exp2B1 =   1.723; /* initialise as B1 */
      CosO1  =   1.997; /* initialise as O(mega)1 */
      break;
    case 1:
      A0     = -0.6472;
      A1     =  -4.531;
      Exp2B0 =   1.527; /* initialise as B0 */
      CosO0  =  0.6719; /* initialise as O(mega)0 */
      C0     =  0.6494;
      C1     =  0.9557;
      Exp2B1 =   1.516; /* initialise as B1 */
      CosO1  =   2.072; /* initialise as O(mega)1 */
      break;
    case 2:
      A0     =  -1.331;
      A1     =   3.661;
      Exp2B0 =    1.24; /* initialise as B0 */
      CosO0  =   0.748; /* initialise as O(mega)0 */
      C0     =  0.3225;
      C1     =  -1.738;
      Exp2B1 =   1.314; /* initialise as B1 */
      CosO1  =   2.166; /* initialise as O(mega)1 */
      break;
  }

  ExpB0  = exp(Exp2B0 / Sigma);
  Exp2B0 = exp(2 * Exp2B0 / Sigma);
  SinO0  = sin(CosO0 / Sigma);
  CosO0  = cos(CosO0 / Sigma);
  Scale0 = 1 + Exp2B0 - 2 * CosO0 * ExpB0;

  ExpB1  = exp(Exp2B1 / Sigma);
  Exp2B1 = exp(2 * Exp2B0 / Sigma);
  SinO1  = sin(CosO1 / Sigma);
  CosO1  = cos(CosO1 / Sigma);
  Scale1 = 1 + Exp2B1 - 2 * CosO1 * ExpB1;

  for(Index = 0; Index < DerivationOrder; Index++) {
    Scale0 *= Scale0;
    Scale1 *= Scale1;
  }

  /* normalisation */
  switch(DerivationOrder) {
    case 0: 
      ScaleA = 2 * A1 * SinO0 * ExpB0 - A0 * (1 - Exp2B0);
      ScaleC = 2 * C1 * SinO1 * ExpB1 - C0 * (1 - Exp2B1);
      break;
    case 1:
      ScaleA = -2 * A1 * SinO0 * ExpB0 * (1 - Exp2B0)
	      + 2 * A0 * ExpB0 * (2 * ExpB0 - CosO0 * (1 + Exp2B0));
      ScaleC = -2 * C1 * SinO1 * ExpB1 * (1 - Exp2B1)
	      + 2 * C0 * ExpB1 * (2 * ExpB1 - CosO1 * (1 + Exp2B1));
      break;
    case 2:
      ScaleA = A1 * SinO0 * ExpB0
	       * (1 + ExpB0 * (2 * CosO0 * (1 + Exp2B0) + Exp2B0 - 6))
	     + A0 * ExpB0
               * (2 * ExpB0 * (2 - CosO0 * CosO0) * (1 - Exp2B0)
		  - CosO0 * (1 - Exp2B0 * Exp2B0));
      ScaleC = C1 * SinO1 * ExpB1
	       * (1 + ExpB1 * (2 * CosO1 * (1 + Exp2B1) + Exp2B1 - 6))
	     + C0 * ExpB1
               * (2 * ExpB1 * (2 - CosO1 * CosO1) * (1 - Exp2B1)
		  - CosO1 * (1 - Exp2B1 * Exp2B1));
      break;
  }

  Scale = ScaleA / Scale0 + ScaleC / Scale1;
  A0 /= Scale;
  A1 /= Scale;
  C0 /= Scale;
  C1 /= Scale;
  Np0 = A0 + C0;
  Np1 = (A1 * SinO0 - (A0 + 2 * C0) * CosO0) / ExpB0
      + (C1 * SinO1 - (C0 + 2 * A0) * CosO1) / ExpB1;
  Np2 = A0 / Exp2B1 + C0 / Exp2B0
      + ((A0 + C0) * CosO0 * CosO1 - A1 * SinO0 * CosO1 - C1 * SinO1 * CosO0)
      * 2 / (ExpB0 * ExpB1);
  Np3 = (A1 * SinO0 - A0 * CosO0) / (ExpB0 * Exp2B1)
      + (C1 * SinO1 - C0 * CosO1) / (ExpB1 * Exp2B0);
  D1 = -2 * CosO0 / ExpB0 - 2 * CosO1 / ExpB1;
  D2 = 1 / Exp2B0 + 1 / Exp2B1 + 4 * CosO0 * CosO1 / (ExpB0 * ExpB1);
  D3 = -2 * CosO0 / (ExpB0 * Exp2B1) - 2 * CosO1 / (ExpB1 * Exp2B0);
  D4 = 1 / (Exp2B0 * Exp2B1);

  switch(DerivationOrder) {
    case 0:
    case 2:
      Nn1 = -D1 * Np0 + Np1;
      Nn2 = -D2 * Np0 + Np2;
      Nn3 = -D3 * Np0 + Np3;
      Nn4 = -D4 * Np0;
      break;
    case 1:
      Nn1 = D1 * Np0 - Np1;
      Nn2 = D2 * Np0 - Np2;
      Nn3 = D3 * Np0 - Np3;
      Nn4 = D4 * Np0;
      break;
  }

  /* allocating intermediate buffers 1 and 2 with as size */
  /* the largest dimension of the map */
  Increment = XD > YD ? (XD > ZD ? XD : ZD) : (YD > ZD ? YD : ZD);
  Intermediate1 = calloc(2 * Increment, sizeof(float));
  OutOfMemory(Intermediate1);
  Intermediate2 = Intermediate1 + Increment;

  /* consecutive convolution with a 1D Deriche Gaussian */
  /* over the X, Y and Z axis, respectively */
  for(AxisIndex = 0; AxisIndex < 3; AxisIndex++) {
    for(z = 0; z < 1 + (ZD - 1) * AxisFlags[AxisIndex]; z++) {
      for(y = 0; y < 1 + (YD - 1) * AxisFlags[AxisIndex + 1]; y++) {
	for(x = 0; x < 1 + (XD - 1) * AxisFlags[AxisIndex + 2]; x++) {
	  Increment = AxisFlags[AxisIndex] * z * XD * YD
	            + AxisFlags[AxisIndex + 1] * y * XD
	            + AxisFlags[AxisIndex + 2] * x;
	  LineStart = PdbDensityValues + Increment;
	  Increment = (1 - AxisFlags[AxisIndex]) * XD * YD
       		    + (1 - AxisFlags[AxisIndex + 1]) * XD
		    + (1 - AxisFlags[AxisIndex + 2]);
	  Dimension = (1 - AxisFlags[AxisIndex]) * ZD
	            + (1 - AxisFlags[AxisIndex + 1]) * YD
                    + (1 - AxisFlags[AxisIndex + 2]) * XD;

      	  /* convolution with the positive half of the filter */

	  /* pointer initialisation */
	  L4 = LineStart;
	  L3 = L4 + Increment;
	  L2 = L3 + Increment;
	  L1 = L2 + Increment;
	  L0 = L1 + Increment;
	  I4 = Intermediate1;
	  I3 = I4 + 1;
	  I2 = I3 + 1;
	  I1 = I2 + 1;
	  I0 = I1 + 1;
	  
	  /* calculation */
	  *I4 = Np0 * *L4;
          *I3 = Np0 * *L3 + Np1 * *L4
              - D1 * *I4;
          *I2 = Np0 * *L2 + Np1 * *L3 + Np2 * *L4
              - D1 * *I3 - D2 * *I4;
          *I1 = Np0 * *L1 + Np1 * *L2 + Np2 * *L3 + Np3 * *L4
              - D1 * *I2 - D2 * *I3 - D3 * *I4;
	  for(Index = 4; Index < Dimension; Index++) {
 	    *I0 = Np0 * *L0 + Np1 * *L1 + Np2 * *L2 + Np3 * *L3
  	 	- D1 * *I1 - D2 * *I2 - D3 * *I3 - D4 * *I4;
	    I0++; I1++; I2++; I3++; I4++;
	    L0 += Increment; L1 += Increment; L2 += Increment; L3 += Increment;
	  }

	  /* convolution with the negative half of the filter */

	  /* pointer initialisation */
          I4 = Intermediate2 + Dimension - 1;
          I3 = I4 - 1;
	  I2 = I3 - 1;
	  I1 = I2 - 1;
	  I0 = I1 - 1;
	  L4 = LineStart;
	  L4 += (Dimension - 1) * Increment;
	  L3 = L4 - Increment;
	  L2 = L3 - Increment;
	  L1 = L2 - Increment;

	  /* calculation */
	  *I4 = 0;
	  *I3 = Nn1 * *L4;
	  *I2 = Nn1 * *L3 + Nn2 * *L4
	      - D1 * *I3;
      	  *I1 = Nn1 * *L2 + Nn2 * *L3 + Nn3 * *L4
              - D1 * *I2 - D2 * *I3;
	  for(Index = Dimension - 5; Index >= 0; Index--) {
      	    *I0 = Nn1 * *L1 + Nn2 * *L2 + Nn3 * *L3 + Nn4 * *L4
                - D1 * *I1 - D2 * *I2 - D3 * *I3 - D4 * *I4;
	    I0--; I1--; I2--; I3--; I4--;
	    L1 -= Increment; L2 -= Increment; L3 -= Increment; L4 -= Increment;
	  }

	  /* exporting the sum of the two halves */
	  I1 = Intermediate1;
	  I2 = Intermediate2;
	  L0 = LineStart;
	  for(Index = 0; Index < Dimension; Index++) {
	    *L0 = *I1 + *I2;
	    I1++; I2++; L0 += Increment;
  }}}}} free(Intermediate1);
}

  float CrossCorrelation(long int *TargetCoord,
		       float *TargetValues,
		       long int VoxelNumber,
		       float ***Template,
		       long int TemplateSectionD,
		       long int TemplateRowD,
		       long int TemplateColumnD,
		       float *ParameterPointer,
		       float TX,
		       float TY,
		       float TZ,
                       float *Coefficients) {
  float *TemplateValues, *TargetPointer, *TemplatePointer;
  float TargetValue, TemplateValue;
  float TemplateDirectMean, TemplateLaplacianMean, TemplateDirectSigma, TemplateLaplacianSigma;
  long int TargetSection, TargetRow, TargetColumn;
  long int TemplateSectionL, TemplateRowL, TemplateColumnL, TemplateSectionU, TemplateRowU, TemplateColumnU;
  float TemplateSectionF, TemplateRowF, TemplateColumnF; /* fractional part */
  long int CoordIndex, TemplateSize;
  float DirectCCC, LaplacianCCC;
  float *LLL, *LLU, *LUL, *LUU, *ULL, *ULU, *UUL, *UUU; 

  TemplateValues = TargetValues + VoxelNumber + VoxelNumber;
  TemplateSize = TemplateSectionD * TemplateRowD * TemplateColumnD;
  TemplateDirectMean = 0;
  TemplateLaplacianMean = 0;
  CoordIndex = VoxelNumber + VoxelNumber + VoxelNumber;
  for(TemplatePointer = TemplateValues + VoxelNumber; TemplatePointer > TemplateValues; ) {
    /* for each target voxel ... */
    TargetColumn = TargetCoord[--CoordIndex];
    TargetRow = TargetCoord[--CoordIndex];
    TargetSection = TargetCoord[--CoordIndex];
    /* calculate the coordinates of the corresponding template voxel ... */
    TemplateSectionF = modff(ParameterPointer[8] * TargetSection +
                             ParameterPointer[7] * TargetRow +
                             ParameterPointer[6] * TargetColumn +
                             TZ, &TemplateValue);
    TemplateSectionL = TemplateValue;
    TemplateSectionU = TemplateSectionL + 1;
    TemplateRowF = modff(ParameterPointer[5] * TargetSection +
                         ParameterPointer[4] * TargetRow +
                         ParameterPointer[3] * TargetColumn +
                         TY, &TemplateValue);
    TemplateRowL = TemplateValue;
    TemplateRowU = TemplateRowL + 1;
    TemplateColumnF = modff(ParameterPointer[2] * TargetSection +
                            ParameterPointer[1] * TargetRow +
                            ParameterPointer[0] * TargetColumn +
                            TX, &TemplateValue);
    TemplateColumnL = TemplateValue;
    TemplateColumnU = TemplateColumnL + 1;
    /* and its trilinearly interpolated value */
    if(TemplateSectionU >= TemplateSectionD ||
       TemplateRowU >= TemplateRowD ||
       TemplateColumnU >= TemplateColumnD ||
       TemplateSectionL < 0 ||
       TemplateRowL < 0 ||
       TemplateColumnL < 0) {
      *Coefficients = -2;
      return;
    } LLU = Template[TemplateSectionL][TemplateRowL];
    LLL = LLU + TemplateColumnL;
    LLU += TemplateColumnU;
    LUU = Template[TemplateSectionL][TemplateRowU];
    LUL = LUU + TemplateColumnL;
    LUU += TemplateColumnU;
    ULU = Template[TemplateSectionU][TemplateRowL];
    ULL = ULU + TemplateColumnL;
    ULU += TemplateColumnU;
    UUU = Template[TemplateSectionU][TemplateRowU];
    UUL = UUU + TemplateColumnL;
    UUU += TemplateColumnU;
    TemplateValue = (1 - TemplateSectionF) *
		      ( (1 - TemplateRowF) *
                        ( (1 - TemplateColumnF) * *LLL
 		        + TemplateColumnF * *LLU)
                      + TemplateSectionF *
                        ( (1 - TemplateColumnF) * *LUL
 		        + TemplateColumnF * *LUU))
                    + TemplateSectionF *
   		      ( (1 - TemplateRowF) *
                        ( (1 - TemplateColumnF) * *ULL
 		        + TemplateColumnF * *ULU)
                      + TemplateSectionF *
                        ( (1 - TemplateColumnF) * *UUL
 		        + TemplateColumnF * *UUU));
    *(--TemplatePointer) = TemplateValue;
    TemplateDirectMean += TemplateValue;
    TemplateValue = (1 - TemplateSectionF) *
		      ( (1 - TemplateRowF) *
                        ( (1 - TemplateColumnF) * *(LLL + TemplateSize)
 		        + TemplateColumnF * *(LLU + TemplateSize))
                      + TemplateSectionF *
                        ( (1 - TemplateColumnF) * *(LUL + TemplateSize)
 		        + TemplateColumnF * *(LUU + TemplateSize)))
                    + TemplateSectionF *
   		      ( (1 - TemplateRowF) *
                        ( (1 - TemplateColumnF) * *(ULL + TemplateSize)
 		        + TemplateColumnF * *(ULU + TemplateSize))
                      + TemplateSectionF *
                        ( (1 - TemplateColumnF) * *(UUL + TemplateSize)
 		        + TemplateColumnF * *(UUU + TemplateSize)));
    *(TemplatePointer + VoxelNumber) = TemplateValue;
    TemplateLaplacianMean += TemplateValue;
  } TemplateDirectMean /= VoxelNumber;
  TemplateLaplacianMean /= VoxelNumber;
  TemplateDirectSigma = 0;
  TemplateLaplacianSigma = 0;
  DirectCCC = 0;
  LaplacianCCC = 0;
  TargetPointer = TargetValues + VoxelNumber;
  for(TemplatePointer = TemplateValues + VoxelNumber; TemplatePointer > TemplateValues; ) {
    TemplateValue = *(--TemplatePointer) - TemplateDirectMean;
    DirectCCC += TemplateValue * *(--TargetPointer);
    TemplateDirectSigma += TemplateValue * TemplateValue;
    TemplateValue = *(TemplatePointer + VoxelNumber) - TemplateLaplacianMean;
    LaplacianCCC += TemplateValue * *(TargetPointer + VoxelNumber);
    TemplateLaplacianSigma += TemplateValue * TemplateValue;
  } Coefficients[0] = DirectCCC / sqrtf(TemplateDirectSigma);
  Coefficients[1] = LaplacianCCC / sqrtf(TemplateLaplacianSigma);
  return;
}

void LaplacianFilter(float ***Density,
		     long int SectionD,
		     long int RowD,
		     long int ColumnD) {
  float *Pointer;
  float Value;
  long int Section, Row, Column;
  long int SectionM, RowM, ColumnM;
  long int SectionP, RowP, ColumnP;
  long int MatrixSize;
  
  MatrixSize = SectionD * RowD * ColumnD;
  for(Section = 0; Section < SectionD; Section++) {
    if(Section == 0) {
      SectionM = 0;
    } else {
      SectionM = Section - 1;
    } SectionP = Section + 1;
    if(SectionP == SectionD) SectionP = Section;
    for(Row = 0; Row < RowD; Row++) {
      if(Row == 0) {
        RowM = 0;
      } else {
        RowM = Row - 1;
      } RowP = Row + 1;
      if(RowP == RowD) RowP = Row;
      for(Column = 0; Column < ColumnD; Column++) {
        if(Column == 0) {
	  ColumnM = Column;
        } else {
	  ColumnM = Column - 1;
        } ColumnP = Column + 1;
        if(ColumnP == ColumnD) ColumnP = Column;
        Pointer = Density[Section][Row] + Column;
        *(Pointer + MatrixSize) = -6 * *Pointer
                                + Density[Section][Row][ColumnM]
                                + Density[Section][Row][ColumnP]
                                + Density[Section][RowM][Column]
                                + Density[Section][RowP][Column]
                                + Density[SectionM][Row][Column]
                                + Density[SectionP][Row][Column];
}}}}

int main(void)
{
  char DensityMap[81]; /* name of the density map file */
  char PdbFile[81]; /* name of atomic structure pdb file */
  float CutOff, VoxelSize, MinSurfaceOverlap;
  long int ToConvertNumber, IncludeMap;
  float Resolution;
  FILE *Map;
  FILE *Model;
  FILE *MapOut;
  FILE *ModelOut;
  FILE *ReportOut;
  char MapPDB[88];
  char Report[87];
  char SolutionPDB[99];
  char *VolumeValues; /* pointer to an array with all the values */
  char ***Volume; /* pointer to an array with pointers to pointers to values */
  char *PdbVolumeValues;
  char ***PdbVolume;
  char *PdbSurfaceValues;
  char ***PdbSurface;
  char *ScoringValues;
  char ***Scoring;
  float *DensityValues;
  float ***Density;
  float *PdbDensityValues;
  float ***PdbDensity;
  long int Section, Row, Column; /* section, row and column indexes */
  long int SectionM, RowM, ColumnM; /* Section, Row and Column - 1 */
  long int SectionP, RowP, ColumnP; /* Section, Row and Column + 1 */
  long int SectionD, RowD, ColumnD; /* map matrix dimensions */
  long int PdbSectionD, PdbRowD, PdbColumnD; /* pdb matrix dimensions */
  float Value;
  long int *Coord;
  long int *PdbCoord;
  long int *Extrema;
  float *Vector;
  long int *PdbVector;
  long int VolumeVoxelCounter, SurfaceVoxelCounter;
  long int PdbVolumeVoxelCounter;
  long int CoordCounter, PdbCoordCounter;
  long int ExtremaCounter;
  long int Index1, Index2, Index3, Index4;
  long int x, y, z;
  float X, Y, Z, MaxX, MinX, MaxY, MinY, MaxZ, MinZ, X2, Y2, Z2, X3, Y3, Z3;
  char Line[101];
  char SixFirstOfLine[7];
  char Coordinates[25];
  float Length;
  float XDif, XDifM, XDifP, YDif, YDifM, YDifP, ZDif, ZDifM, ZDifP;
  float Ra11, Ra12, Ra13, Ra21, Ra22, Ra23, Ra31, Ra32, Ra33;
  float Rb11, Rb12, Rb13, Rb21, Rb22, Rb23, Rb31, Rb32, Rb33;
  float Rc11, Rc12, Rc13, Rc21, Rc22, Rc23, Rc31, Rc32, Rc33;
  float TaX, TaY, TaZ, TbX, TbY, TbZ, TcX, TcY, TcZ;
  float CosAngle, SinAngle, OneMinCosAngle;
  float XSinAngle, YSinAngle, ZSinAngle;
  float *TrigValues; /* rotation step cos(angle) and sin(angle) values */
  const float TwoPi = 6.28318530717958;
  long int TresholdScore;
  long int NumberOfScoreClasses;
  long int *ScoreClassCounters;
  float **ScoreClasses;
  long int AllowedZeroSteps;
  long int SolutionCounter;
  register long int Score;
  long int ScoreU, ScoreD, ScoreN, ScoreE, ScoreS, ScoreW;
  float *ParameterPointer;
  float *Solutions;
  long int DerivationOrder;
  float MinPdbDensity, MaxPdbDensity;
  long int *PdbDensityCoord;
  float *PdbFinalValues;

  long int RotationSteps = 40; /* determines the coverage of rotational space */
  float SignificantMSD = 4; /* smallest significant mean square deviation */ 

  /* get the density map file name */
  printf("\nInput density map file : ");
  scanf("%s", DensityMap);
  Map = fopen(DensityMap, "rb");
  if(Map == NULL) {
    printf("\nError : cannot open %s\n", DensityMap);
    exit(0);
  }

  /* read the interesting part of the header of the binary density map file */
  /* and throw away the rest */
  fread((char *)&ColumnD, 1, 4, Map); /* number of columns */
  fread((char *)&RowD, 1, 4, Map); /* number of rows */
  fread((char *)&SectionD, 1, 4, Map); /* number of sections */
  fread((char *)&Index1, 1, 4, Map); /* data mode */
  /* verify that the data mode is float (2) */
  /* determine simultaneously byte order compatibility */
  if(Index1 == 2) {
    Score = 0;
  } else {
    if(Index1 == 33554432) {
      Score = 1;
    } else {
    printf("\nError : the density values are not stored as floats\n");
    exit(0);
  }} fread(&Line, 1, 24, Map);
  fread((char *)&X, 1, 4, Map); /* cell width in Angstrom */
  fread((char *)&Y, 1, 4, Map); /* cell heigth in Angstrom */
  fread((char *)&Z, 1, 4, Map); /* cell depth in Angstrom */
  fread((char *)&XSinAngle, 1, 4, Map); /* cell angle alpha */
  fread((char *)&YSinAngle, 1, 4, Map); /* cell angle beta */
  fread((char *)&ZSinAngle, 1, 4, Map); /* cell angle gamma */
  fread((char *)&Column, 1, 4, Map); /* column axis 1:x, 2:y, 3:z */
  fread((char *)&Row, 1, 4, Map); /* row axis 1:x, 2:y, 3:z */
  fread((char *)&Section, 1, 4, Map); /* section axis 1:x, 2:y, 3:z */
  fread((char *)&X2, 1, 4, Map); /* min density value */
  fread((char *)&Y2, 1, 4, Map); /* max density value */
  fread((char *)&Z2, 1, 4, Map); /* mean density value */
  fread(&Line, 1, 4, Map);
  fread((char *)&AllowedZeroSteps, 1, 4, Map); /* bytes between header and map */
  fread(&Line, 1, 19, Map);
  for(Index1 = 0; Index1 < 9; Index1++) {
    fread(&Line, 1, 101, Map);
  } 

  /* if required convert the information to native byte order */
  if(Score) {
    ColumnD = LongIntByteOrderConvert(ColumnD);
    RowD = LongIntByteOrderConvert(RowD);
    SectionD = LongIntByteOrderConvert(SectionD);
    X = FloatByteOrderConvert(X); /* unused for now */
    Y = FloatByteOrderConvert(Y); /* unused for now */
    Z = FloatByteOrderConvert(Z); /* unused for now */
    XSinAngle = FloatByteOrderConvert(XSinAngle);
    YSinAngle = FloatByteOrderConvert(YSinAngle);
    ZSinAngle = FloatByteOrderConvert(ZSinAngle);
    Column = LongIntByteOrderConvert(Column);
    Row = LongIntByteOrderConvert(Row);
    Section = LongIntByteOrderConvert(Section);
    X2 = FloatByteOrderConvert(X2);
    Y2 = FloatByteOrderConvert(Y2);
    Z2 = FloatByteOrderConvert(Z2);
    AllowedZeroSteps = LongIntByteOrderConvert(AllowedZeroSteps);
  }

  /* verify that the coordinate system is orthogonal */
  if(!(XSinAngle == 90.0 && YSinAngle == 90.0 && ZSinAngle == 90.0)) {
    printf("\nError : the coordinate system is not orthogonal\n");
    exit(0);
  }

  /* throw away the symmetry operators stored between the header and the map, if any */  
  for(Index1 = 0; Index1 < AllowedZeroSteps; Index1++) {
    fread(&SixFirstOfLine[0], 1, 1, Map);
  }

  /* Get the rest of the required input */
  printf("\nInput PDB file : ");
  scanf("%s", PdbFile);
  Model = fopen(PdbFile, "r");
  if(Model == NULL) {
    printf("Error : cannot open %s\n", PdbFile);
    exit(0);
  } printf("\nInput the cut-off value for inclusion in the rigid EM body (must be between %f and %f) : ", X2, Y2);
  scanf("%f", &CutOff);
  if(CutOff < X2 || CutOff > Y2) {
    printf("\nError : improper cut-off\n");
    exit(0);
  } printf("\nInput voxel size in Angstroms : ");
  scanf("%f", &VoxelSize);
  printf("\nInput the estimated resolution (>= 4/3 voxel size) of the map in Angstrom : ");
  scanf("%f", &Resolution);
  /*  printf("\nInput the minimum percent of the model surface that must be superimposed onto the map surface : ");
  scanf("%f", &MinSurfaceOverlap); 
  MinSurfaceOverlap /= 100; */
  /*******************************************************/
  /* currently MinSurfaceOverlap is set to a standard 20 */
  /*******************************************************/
  MinSurfaceOverlap = 0.2;
  printf("\nInput the number of solutions to be converted into PDB files : ");
  scanf("%d", &ToConvertNumber);
  printf("\nMust the iso-surface shell be included in the output files (y/n) : ");
  scanf("%s", SolutionPDB);
  if(SolutionPDB[0] == 'y') {
    IncludeMap = 1;
  } else {
    if(SolutionPDB[0] == 'n') {
      IncludeMap = 0;
    } else {
      printf("\nError : answer must be yes or no\n");
      exit(0);
  }} printf("\nInput the job identifier that will be part of the output file names : ");
  scanf("%s", SolutionPDB);
  strcpy(MapPDB, SolutionPDB);
  strcat(MapPDB, "Map.pdb");
  strcpy(Report, SolutionPDB);
  strcat(Report, "Report");
  strcat(SolutionPDB, "Solution");

  /* convert to the standard XYZ constellation if necessary */
  if(Section == 3) {
    if(Row == 2) {
      if(Column == 1) {
	TresholdScore = 0;
      } else {
        printf("Error : improper axis constellation\n");
	exit(0);
    }} else {
      if(Row == 1) {
	if(Column == 2) {
	  TresholdScore = 1;
	  y = ColumnD;
	  ColumnD = RowD;
	  RowD = y;
	} else {
	  printf("Error : improper axis constellation\n");
	  exit(0);
      }} else {
	printf("Error : improper axis constellation\n");
	exit(0);
  }}} else {
    if(Section == 1) {
      if(Row == 2) {
	if(Column == 3) {
	  TresholdScore = 2;
	  z = ColumnD;
	  ColumnD = SectionD;
	  SectionD = z;
	} else {
	  printf("Error : improper axis constellation\n");
	  exit(0);
      }} else {
	if(Row == 3) {
	  if(Column == 2) {
	    TresholdScore = 3;
	    y = ColumnD;
	    ColumnD = SectionD;
	    SectionD = RowD;
	    RowD = y;
	  } else {
	    printf("Error : improper axis constellation\n");
	    exit(0);
	}} else {
	  printf("Error : improper axis constellation\n");
	  exit(0);
    }}} else {
      if(Section == 2) {
	if(Row == 3) {
	  if(Column == 1) {
	    TresholdScore = 4;
	    z = RowD;
	    RowD = SectionD;
	    SectionD = z;
	  } else {
	    printf("Error : improper axis constellation\n");
	    exit(0);
	}} else {
	  if(Row == 1) {
	    if(Column == 3) {
	      TresholdScore = 5;
	      z = ColumnD;
	      ColumnD = RowD;
	      RowD = SectionD;
	      SectionD = z;
	    } else {
	      printf("Error : improper axis constellation\n");
	      exit(0);
	  }} else {
	    printf("Error : improper axis constellation\n");
	    exit(0);
      }}} else {
	printf("Error : improper axis constellation\n");
  }}}

  /* initialise the density 3D matrix */
  DensityValues = calloc(2 * SectionD * RowD * ColumnD, sizeof(float));
  OutOfMemory(DensityValues);
  Density = calloc(SectionD, sizeof(float **));
  for(Section = 0; Section < SectionD; Section++) {
    Density[Section] = calloc(RowD, sizeof(float *));
    OutOfMemory(Density[Section]);
    for(Row = 0; Row < RowD; Row++) {
    Density[Section][Row] = DensityValues + ColumnD * (Section * RowD + Row);
  }}
    
  /* initialise three 3D matrices of the required size */
  /* (dimensions + empty voxel borders at all sides) */
  ColumnD += 2;
  RowD +=2;
  SectionD += 2;
  VolumeValues = calloc(SectionD * RowD * ColumnD, sizeof(char));
  OutOfMemory(VolumeValues);
  ScoringValues = calloc(SectionD * RowD * ColumnD, sizeof(char));
  OutOfMemory(ScoringValues);
  Volume = calloc(SectionD, sizeof(char **));
  OutOfMemory(Volume);
  Scoring = calloc(SectionD, sizeof(char **));
  OutOfMemory(Scoring);
  for(Section = 0; Section < SectionD; Section++) {
    Volume[Section] = calloc(RowD, sizeof(char *));
    OutOfMemory(Volume[Section]);
    Scoring[Section] = calloc(RowD, sizeof(char *));
    OutOfMemory(Scoring[Section]);
    for(Row = 0; Row < RowD; Row++) {
      Volume[Section][Row] = VolumeValues + ColumnD * (Section * RowD + Row);
      Scoring[Section][Row] = ScoringValues + ColumnD * (Section * RowD + Row);
  }}

  /* fill the volume array up with bits (1 if the corresponding density value is > CutOff) and close map */
  VolumeVoxelCounter = 0; /* counts how many voxels in the volume */

  switch(TresholdScore) {
    case 0:
      for(Section = 1; Section < (SectionD - 1); Section++) {
	SectionM = Section - 1;
        for(Row = 1; Row < (RowD - 1); Row++) {
	  RowM = Row - 1;
          for(Column = 1; Column < (ColumnD - 1); Column++) {
            fread((char *)&Value, 1, 4, Map);
            if(Score) Value = FloatByteOrderConvert(Value); 
	    Density[SectionM][RowM][Column - 1] = Value;
            if(Value >= CutOff) {
            Volume[Section][Row][Column] = 1;
            ++VolumeVoxelCounter;
      }}}} break;
    case 1:
      for(Section = 1; Section < (SectionD - 1); Section++) {
	SectionM = Section -1;
        for(Column = 1; Column < (ColumnD - 1); Column++) {
	  ColumnM = Column - 1;
          for(Row = 1; Row < (RowD - 1); Row++) {
            fread((char *)&Value, 1, 4, Map);
            if(Score) Value = FloatByteOrderConvert(Value);
	    Density[SectionM][Row - 1][ColumnM] = Value; 
            if(Value >= CutOff) {
            Volume[Section][Row][Column] = 1;
            ++VolumeVoxelCounter;
      }}}} break;
    case 2:
      for(Column = 1; Column < (ColumnD - 1); Column++) {
	ColumnM = Column - 1;
        for(Row = 1; Row < (RowD - 1); Row++) {
	  RowM = Row - 1;
          for(Section = 1; Section < (SectionD - 1); Section++) {
            fread((char *)&Value, 1, 4, Map);
            if(Score) Value = FloatByteOrderConvert(Value);
	    Density[Section - 1][RowM][ColumnM] = Value; 
            if(Value >= CutOff) {
            Volume[Section][Row][Column] = 1;
            ++VolumeVoxelCounter;
      }}}} break;
    case 3:
      for(Column = 1; Column < (ColumnD - 1); Column++) {
	ColumnM = Column - 1;
        for(Section = 1; Section < (SectionD - 1); Section++) {
	  SectionM = Section - 1;
          for(Row = 1; Row < (RowD - 1); Row++) {
            fread((char *)&Value, 1, 4, Map);
            if(Score) Value = FloatByteOrderConvert(Value);
 	    Density[SectionM][Row - 1][ColumnM] = Value;
            if(Value >= CutOff) {
            Volume[Section][Row][Column] = 1;
            ++VolumeVoxelCounter;
      }}}} break;
    case 4:
      for(Row = 1; Row < (RowD - 1); Row++) {
	RowM = Row - 1;
        for(Section = 1; Section < (SectionD - 1); Section++) {
	  SectionM = Section - 1;
          for(Column = 1; Column < (ColumnD - 1); Column++) {
            fread((char *)&Value, 1, 4, Map);
            if(Score) Value = FloatByteOrderConvert(Value);
 	    Density[SectionM][RowM][Column - 1] = Value;
            if(Value >= CutOff) {
            Volume[Section][Row][Column] = 1;
            ++VolumeVoxelCounter;
      }}}} break;
    case 5:
      for(Row = 1; Row < (RowD - 1); Row++) {
	RowM = Row - 1;
        for(Column = 1; Column < (ColumnD - 1); Column++) {
	  ColumnM = Column - 1;
          for(Section = 1; Section < (SectionD - 1); Section++) {
            fread((char *)&Value, 1, 4, Map);
            if(Score) Value = FloatByteOrderConvert(Value);
 	    Density[Section - 1][RowM][ColumnM] = Value;
            if(Value >= CutOff) {
            Volume[Section][Row][Column] = 1;
            ++VolumeVoxelCounter;
      }}}} break;
  } fclose(Map);

  /* Laplacian filtering of the density map */
  LaplacianFilter(Density,
	          SectionD - 2,
	          RowD - 2,
	          ColumnD - 2);

  /* fill the score array up with bits, */
  /* i.e. surface voxels + their 6 3D orthogonal neighbours */
  /* produce a pdb file that represents the surface of the volume in the map */
  /* for all voxel points of the density map surface, */
  /* calculate an approximation of the normalised inward pointing vector */
  /* orthogonal to the tangent plane */
  /* (= vector sum of direction vectors to all surrounding voxels */
  /* make a list of the surface voxel coordinates and one with corresponding */
  /* vector coordinates (voxels associated with a null vector are left out) */
  X = (ColumnD - 2) / 2; /* map center X coordinate */
  Y = (RowD - 2) / 2; /* map center Y coordinate */
  Z = (SectionD - 2) / 2; /* map center Z coordinate */
  MapOut = fopen(MapPDB, "wb");
  SurfaceVoxelCounter = 0;
  ExtremaCounter = 1; /* serves here as a atom counter */
  CoordCounter = 0; /* Counts how many voxels in the coord and vector arrays */
  Coord = NULL;
  Vector = NULL;
  for(Section = 1; Section < (SectionD - 1); Section++) {
    SectionM = Section - 1;
    SectionP = Section + 1;
    for(Row = 1; Row < (RowD - 1); Row++) {
      RowM = Row - 1;
      RowP = Row + 1;
      for(Column = 1; Column < (ColumnD - 1); Column++) {
  	ColumnM = Column - 1;
	ColumnP = Column + 1;

	/* if surface voxel */            
        if(Volume[Section][Row][Column]&&(!(
          Volume[Section][Row][ColumnP]&&
          Volume[Section][Row][ColumnM]&&
          Volume[Section][RowP][Column]&&
          Volume[Section][RowM][Column]&&
          Volume[SectionP][Row][Column]&&
          Volume[SectionM][Row][Column]))) {

	    /* write a pseudoatom to the {JobId}Map.pdb file */
	    sprintf(Line, "HETATM%5d MAP  MAP %5d    %8.3f%8.3f%8.3f\n",
	     ExtremaCounter, ExtremaCounter, (Column - (1 + X)) * VoxelSize, (Row - (1 + Y)) * VoxelSize, (Section - (1 + Z)) * VoxelSize);
	    fwrite(&Line, 1, strlen(Line), MapOut);
  	    ++ExtremaCounter;
   	    ++SurfaceVoxelCounter;
	    if(ExtremaCounter == 10000) ExtremaCounter = 1;

            /* switch required bits in surface and score matrices */
            Scoring[SectionM][Row][Column] = 1;
 	    Scoring[Section][RowM][Column] = 1;
 	    Scoring[Section][Row][ColumnM] = 1;
 	    Scoring[Section][Row][Column] = 1;
 	    Scoring[Section][Row][ColumnP] = 1;
 	    Scoring[Section][RowP][Column] = 1;
	    Scoring[SectionP][Row][Column] = 1;

            /* calculate the orthogonal vector */
            z = 
               -Volume[SectionM][RowM][ColumnM]
              - Volume[SectionM][RowM][Column]
              - Volume[SectionM][RowM][ColumnP]
              - Volume[SectionM][Row][ColumnM]
              - Volume[SectionM][Row][Column]
              - Volume[SectionM][Row][ColumnP]
              - Volume[SectionM][RowP][ColumnM]
              - Volume[SectionM][RowP][Column]
              - Volume[SectionM][RowP][ColumnP]
              + Volume[SectionP][RowM][ColumnM]
              + Volume[SectionP][RowM][Column]
              + Volume[SectionP][RowM][ColumnP]
              + Volume[SectionP][Row][ColumnM]
              + Volume[SectionP][Row][Column]
              + Volume[SectionP][Row][ColumnP]
              + Volume[SectionP][RowP][ColumnM]
              + Volume[SectionP][RowP][Column]
              + Volume[SectionP][RowP][ColumnP];
            y = 
               -Volume[SectionM][RowM][ColumnM]
              - Volume[SectionM][RowM][Column]
              - Volume[SectionM][RowM][ColumnP]
              + Volume[SectionM][RowP][ColumnM]
              + Volume[SectionM][RowP][Column]
              + Volume[SectionM][RowP][ColumnP]
              - Volume[Section][RowM][ColumnM]
              - Volume[Section][RowM][Column]
              - Volume[Section][RowM][ColumnP]
              + Volume[Section][RowP][ColumnM]
              + Volume[Section][RowP][Column]
              + Volume[Section][RowP][ColumnP]
              - Volume[SectionP][RowM][ColumnM]
              - Volume[SectionP][RowM][Column]
              - Volume[SectionP][RowM][ColumnP]
              + Volume[SectionP][RowP][ColumnM]
              + Volume[SectionP][RowP][Column]
              + Volume[SectionP][RowP][ColumnP];
            x = 
               -Volume[SectionM][RowM][ColumnM]
              + Volume[SectionM][RowM][ColumnP]
              - Volume[SectionM][Row][ColumnM]
              + Volume[SectionM][Row][ColumnP] 
              - Volume[SectionM][RowP][ColumnM]
              + Volume[SectionM][RowP][ColumnP]
              - Volume[Section][RowM][ColumnM]
              + Volume[Section][RowM][ColumnP]
              - Volume[Section][Row][ColumnM]
              + Volume[Section][Row][ColumnP]
              - Volume[Section][RowP][ColumnM]
              + Volume[Section][RowP][ColumnP]
              - Volume[SectionP][RowM][ColumnM]
              + Volume[SectionP][RowM][ColumnP]
              - Volume[SectionP][Row][ColumnM]
              + Volume[SectionP][Row][ColumnP]
              - Volume[SectionP][RowP][ColumnM]
              + Volume[SectionP][RowP][ColumnP];
            Length = sqrtf(z * z + y * y + x * x);

            /* if the vector is not the zero vector, normalise it */
            if(Length) {
	      CoordCounter += 3;
   	      Coord = realloc(Coord, CoordCounter * sizeof(long int));
	      Coord[CoordCounter - 1] = Column;
	      Coord[CoordCounter - 2] = Row;
	      Coord[CoordCounter - 3] = Section;
   	      Vector = realloc(Vector, CoordCounter * sizeof(float));
	      Vector[CoordCounter - 1] = x / Length;
	      Vector[CoordCounter - 2] = y / Length;
	      Vector[CoordCounter - 3] = z / Length; 
  }}}}} fclose(MapOut);
  CoordCounter -= 3;
  printf("\nMap dimension             : %d x %d x %d\nNumber of volume voxels   : %d\nNumber of surface voxels  : %d\n",
	 ColumnD - 2, RowD - 2, SectionD - 2, VolumeVoxelCounter, SurfaceVoxelCounter);

  /* free the memory allocated to the 3D volume array */
  free(VolumeValues);
  for(Section = 0; Section < SectionD; Section++) {
    free(Volume[Section]);
  } free(Volume);

  /* load the model */
  SixFirstOfLine[6] = '\0';
  Coordinates[24] = '\0';
  PdbCoordCounter = 4; /* serves as an index for the coordinate array */
  Solutions = malloc(4 * sizeof(float)); /* pdb file atom coordinates and number */
  OutOfMemory(Solutions);

  /* initialise (Max/Min)(X/Y/Z) with the first atom coordinates */
  while(fgets(Line, sizeof(Line), Model)) {
    memcpy(SixFirstOfLine, Line, 6);
    if(!strcmp(SixFirstOfLine, "ATOM  ")) break;    
  } memcpy(Coordinates, Line + 30, 24);
  sscanf(Coordinates, "%8f%8f%8f", &X, &Y, &Z);
  Solutions[0] = Z;
  Solutions[1] = Y;
  Solutions[2] = X;
  Solutions[3] = AtomToAtomicNumber(Line + 13);
  MaxX = X;
  MinX = X;
  MaxY = Y;
  MinY = Y;
  MaxZ = Z;
  MinZ = Z;

  /* parse all the remaining atom coordinates and determine (Max/Min)(X/Y/Z) */
  while(fgets(Line, sizeof(Line), Model)) {
    memcpy(SixFirstOfLine, Line, 6);
    if(!strcmp(SixFirstOfLine, "ATOM  ")) {
      memcpy(Coordinates, Line + 30, 24);
      sscanf(Coordinates, "%8f%8f%8f", &X, &Y, &Z);
      switch(Line[13]) {
        case 'C' :
        case 'O' :
        case 'N' :
        case 'P' :
        case 'S' :
          PdbCoordCounter += 4;
          Solutions = realloc(Solutions, PdbCoordCounter * sizeof(float));
          Solutions[PdbCoordCounter - 1] = AtomToAtomicNumber(Line + 13);
          Solutions[PdbCoordCounter - 2] = X;
          Solutions[PdbCoordCounter - 3] = Y;
          Solutions[PdbCoordCounter - 4] = Z;
          if(X > MaxX) {
	    MaxX = X;
	  } else {
	    if(X < MinX) MinX = X;
          } if(Y > MaxY) {
	    MaxY = Y;
	  } else {
	    if(Y < MinY) MinY = Y;
          } if(Z > MaxZ) {
	    MaxZ = Z;
	  } else {
	    if(Z < MinZ) MinZ = Z;
 	  } break;
  }}} fclose(Model);
  PdbCoordCounter -= 4;

  /* calculate dimension of the model matrices (+ borders), int by integer casting */
  PdbSectionD = 3.5 + (MaxZ - MinZ) / VoxelSize;
  PdbRowD = 3.5 + (MaxY - MinY) / VoxelSize;
  PdbColumnD = 3.5 + (MaxX - MinX) / VoxelSize;

  /* initialise a float matrix and two char matrices of the required size */
  x = PdbSectionD * PdbRowD * PdbColumnD;
  PdbDensityValues = calloc(x, sizeof(float));
  OutOfMemory(PdbDensityValues);
  PdbVolumeValues = calloc(x, sizeof(char));
  OutOfMemory(PdbVolumeValues);
  PdbSurfaceValues = calloc(x, sizeof(char));
  OutOfMemory(PdbSurfaceValues);
  PdbDensity = calloc(PdbSectionD, sizeof(float **));
  OutOfMemory(PdbDensity);
  PdbVolume = calloc(PdbSectionD, sizeof(char **));
  OutOfMemory(PdbVolume);
  PdbSurface = calloc(PdbSectionD, sizeof(char **));
  OutOfMemory(PdbSurface);
  for(Section = 0; Section < PdbSectionD; Section++) {
    PdbDensity[Section] = malloc(PdbRowD * sizeof(float *));
    OutOfMemory(PdbDensity[Section]);
    PdbVolume[Section] = malloc(PdbRowD * sizeof(char *));
    OutOfMemory(PdbVolume[Section]);
    PdbSurface[Section] = malloc(PdbRowD * sizeof(char *));
    OutOfMemory(PdbSurface[Section]);
    y = Section * PdbRowD;
    for(Row = 0; Row < PdbRowD; Row++) {
      z = PdbColumnD * (y + Row);
      PdbDensity[Section][Row] = PdbDensityValues + z;
      PdbVolume[Section][Row] = PdbVolumeValues + z;
      PdbSurface[Section][Row] = PdbSurfaceValues + z;
  }}
  
  /* fill the surface (as pre-volume) and density arrays up  */
  for(Index1 = 0; Index1 < PdbCoordCounter; Index1 += 4) {
    /* int by integer casting */
    Section = 1.5 + (Solutions[Index1] - MinZ) / VoxelSize;
    SectionP = Section + 1;
    Row = 1.5 + (Solutions[Index1 + 1] - MinY) / VoxelSize;
    RowP = Row + 1;
    Column = 1.5 + (Solutions[Index1 + 2] - MinX) / VoxelSize;
    ColumnP = Column + 1;
    PdbDensity[Section][Row][Column] += Solutions[Index1 + 3];
    PdbSurface[Section][Row][Column] = 1;
    if(VoxelSize < 3.5) {
      PdbVolume[Section][Row][Column] = 1;
      PdbVolume[Section][Row][ColumnP] = 1;
      PdbVolume[Section][RowP][Column] = 1; 
      PdbVolume[Section][RowP][ColumnP] = 1;
      PdbVolume[SectionP][Row][Column] = 1;
      PdbVolume[SectionP][Row][ColumnP] = 1;
      PdbVolume[SectionP][RowP][Column] = 1; 
      PdbVolume[SectionP][RowP][ColumnP] = 1;
  }}

  /* calculate the number of voxels filled and the center of mass */
  /* (all voxels are weighed evenly) */
  Index1 = 0; /* serves as sum of all voxel Z coordinates */
  Index2 = 0; /* serves as sum of all voxel Y coordinates */
  Index3 = 0; /* serves as sum of all voxel X coordinates */
  PdbVolumeVoxelCounter = 0;
  if(VoxelSize < 3.5) Index4 = 0; 
  for(Section = 1; Section < (PdbSectionD - 1); Section++) {
    for(Row = 1; Row < (PdbRowD - 1); Row++) {
      for(Column = 1; Column < (PdbColumnD - 1); Column++) {
	if(PdbSurface[Section][Row][Column]) {
          Index1 += Section;
	  Index2 += Row;
	  Index3 += Column;
          ++PdbVolumeVoxelCounter;
        } if(VoxelSize < 3.5 && PdbVolume[Section][Row][Column]) {
          ++Index4;
          PdbVolume[Section][Row][Column] = 0;
  }}}} Value = PdbVolumeVoxelCounter; /* cast integer to float */
  Z = Index1 / Value; /* Z coordinate of center of mass */
  Y = Index2 / Value; /* Y coordinate of center of mass */
  X = Index3 / Value; /* X coordinate of center of mass */
  if(VoxelSize < 3.5) PdbVolumeVoxelCounter += (Index4 - PdbVolumeVoxelCounter) * (3.5 / VoxelSize - 1);

  /* convolve PdbDensity with the recursive Deriche's Gaussian filter */
  /* and calculate the resulting minimum and maximum values */
  DerivationOrder = 0;
  ConvolveDericheGaussian(PdbDensityValues, PdbSectionD, PdbRowD, PdbColumnD, Resolution / (2 * VoxelSize), DerivationOrder);
  X2 = *PdbDensityValues; /* temporarily used as minimal density */
  Y2 = X2; /* temporarily used as maximal density */
  x = PdbSectionD * PdbRowD * PdbColumnD - 1;
  for(ParameterPointer = PdbDensityValues + x; ParameterPointer >= PdbDensityValues; ParameterPointer--) {
    Value = *ParameterPointer;
    if(Value > Y2) {
      Y2 = Value;
    } else {
      if(Value < X2) X2 = Value;
  }}

  /* estimate the iso-surface that circumscribes a number of voxels */
  /* equal to that in the pre-volume (accurate to 1 / 100 000 of the */
  /* density range) */
  PdbCoord = calloc(100001, sizeof(long int)); /* temporary histogram */
  OutOfMemory(PdbCoord);
  Value = (Y2 - X2) / 100000;
  x = PdbSectionD * PdbRowD * PdbColumnD - 1;
  for(ParameterPointer = PdbDensityValues + x; ParameterPointer >= PdbDensityValues; ParameterPointer--) {
    Score = (*ParameterPointer - X2) / Value;
    ++PdbCoord[Score];
  } PdbCoordCounter = 0;
  Index1 = 100001;
  while(PdbCoordCounter < PdbVolumeVoxelCounter) PdbCoordCounter += PdbCoord[--Index1];
  /* adjust counter to the number of iso-surface contained voxels */
  PdbVolumeVoxelCounter = PdbCoordCounter;
  X2 += Index1 * Value; /* lower boundary for the cut-off value */

  /* flip the bits of the iso-surface contained voxels in the volume array */
  /* make an array containing the coordinates of these voxels and one containing */
  /* the corresponding values normalised over the volume */
  PdbDensityCoord = malloc(PdbVolumeVoxelCounter * 3 * sizeof(long int));
  OutOfMemory(PdbDensityCoord);
  PdbFinalValues = malloc(PdbVolumeVoxelCounter * 4 * sizeof(long int));
  OutOfMemory(PdbFinalValues);
  Index1 = 0;
  Index2 = 0;
  Index3 = PdbVolumeVoxelCounter;
  Y2 = 0; /* temporarily used to calculate the mean unfiltered density */
  Y3 = 0; /* temporarily used to calculate the mean Laplacian-filtered density */
  for(Section = 0; Section < PdbSectionD; Section++) {
    for(Row = 0; Row < PdbRowD; Row++) {
      for(Column = 0; Column < PdbColumnD; Column++) {
	Value = PdbDensity[Section][Row][Column]; /* first unfiltered density */
	if(Value >= X2) {
          if(Section == 0) {
            SectionM = 0;
          } else {
            SectionM = Section - 1;
          } SectionP = Section + 1;
          if(SectionP == PdbSectionD) SectionP = Section;
          if(Row == 0) {
            RowM = 0;
          } else {
            RowM = Row - 1;
          } RowP = Row + 1;
          if(RowP == PdbRowD) RowP = Row;
          if(Column == 0) {
            ColumnM = 0;
          } else {
            ColumnM = Column - 1;
          } ColumnP = Column + 1;
          if(ColumnP == PdbColumnD) ColumnP = Column;
	  PdbVolume[Section][Row][Column] = 1;
	  PdbDensityCoord[Index1++] = Section;
	  PdbDensityCoord[Index1++] = Row;
	  PdbDensityCoord[Index1++] = Column;
	  PdbFinalValues[Index2++] = Value;
	  Y2 += Value;
          /* switch to Laplacian-filtered density */
          Value = -6 * Value
                  + PdbDensity[Section][Row][ColumnM]
                  + PdbDensity[Section][Row][ColumnP]
                  + PdbDensity[Section][RowM][Column]
                  + PdbDensity[Section][RowP][Column]
                  + PdbDensity[SectionM][Row][Column]
	          + PdbDensity[SectionP][Row][Column];
          PdbFinalValues[Index3++] = Value;
	  Y3 += Value;
  }}}} Y2 /= PdbVolumeVoxelCounter;
  Y3 /= PdbVolumeVoxelCounter;
 
  /* the 3D density matrix can now be dismantled */
  for(Section = 0; Section < PdbSectionD; Section++) free(PdbDensity[Section]);
  free(PdbDensity);
  free(PdbDensityValues);

  /* normalisation of the iso-surface contained voxels */
  Z2 = 0; /* temporarily used to calculate the unfiltered standard deviation */
  Z3 = 0; /* temporarily used to calculate the Laplacian-filtered standard deviation */
  for(ParameterPointer = PdbFinalValues + PdbVolumeVoxelCounter; ParameterPointer > PdbFinalValues;) {
    Value = *(--ParameterPointer) - Y2;
    *ParameterPointer = Value;
    Z2 += Value * Value;
    Value = *(ParameterPointer + PdbVolumeVoxelCounter) - Y3;
    *(ParameterPointer + PdbVolumeVoxelCounter) = Value;
    Z3 += Value * Value;
  } Z2 = sqrtf(Z2);
  Z3 = sqrtf(Z3);
  for(ParameterPointer = PdbFinalValues + PdbVolumeVoxelCounter; ParameterPointer > PdbFinalValues;) {
    *(--ParameterPointer) /= Z2;
    *(ParameterPointer + PdbVolumeVoxelCounter) /= Z3;
  }

  /* fill the surface array up with bits */
  /* (1 if E volume and makes contact with at least one null-score) */
  /* make a list of surface voxels */
  PdbCoordCounter = 0; /* Counts how many voxels in the surface */
  PdbCoord = malloc(3 * sizeof(long int)); /* coordinates of surface voxels */
  OutOfMemory(PdbCoord);
  for(Section = 0; Section < PdbSectionD; Section++) {
    SectionM = Section - 1;
    SectionP = Section + 1;
    for(Row = 0; Row < PdbRowD; Row++) {
      RowM = Row - 1;
      RowP = Row + 1;
      for(Column = 0; Column < PdbColumnD; Column++) {
  	ColumnM = Column - 1;
	ColumnP = Column + 1;
        if(PdbVolume[Section][Row][Column] &&
           (Section == 0 ||
            Section == PdbSectionD - 1 ||
            Row == 0 ||
            Row == PdbRowD - 1 ||
            Column == 0 ||
            Column == PdbColumnD - 1 || !(
            PdbVolume[Section][Row][ColumnP] &&
            PdbVolume[Section][Row][ColumnM] &&
            PdbVolume[Section][RowP][Column] &&
            PdbVolume[Section][RowM][Column] &&
            PdbVolume[SectionP][Row][Column] &&
            PdbVolume[SectionM][Row][Column]))) {        
          PdbSurface[Section][Row][Column] = 1;
          PdbCoordCounter += 3;
          PdbCoord = realloc(PdbCoord, PdbCoordCounter * sizeof(long int));
	  OutOfMemory(PdbCoord);
          PdbCoord[PdbCoordCounter - 1] = Column;
	  PdbCoord[PdbCoordCounter - 2] = Row;
	  PdbCoord[PdbCoordCounter - 3] = Section;
        } else {
	  PdbSurface[Section][Row][Column] = 0;
  }}}} printf("\nModel dimension           : %d x %d x %d\nNumber of volume voxels   : %d\nNumber of surface voxels  : %d\n",
	      PdbColumnD, PdbRowD, PdbSectionD, PdbVolumeVoxelCounter, PdbCoordCounter / 3);

  /* reduction of model surface voxels to a set of local polar extrema */  
  /* make list of corresponding orthogonal vectors */
  ExtremaCounter = 0;
  Extrema = NULL;
  PdbVector = NULL;
  for(Index1 = 0; Index1 < PdbCoordCounter; Index1 += 3) {
    Section = PdbCoord[Index1];
    if(Section == 0) {
      SectionM = 0;
    } else {
      SectionM = Section - 1;
    } SectionP = Section + 1;
    if(SectionP == PdbSectionD) SectionP = Section;
    Row = PdbCoord[Index1 + 1];
    if(Row == 0) {
      RowM = 0;
    } else {
      RowM = Row - 1;
    } RowP = Row + 1;
    if(RowP == PdbRowD) RowP = Row; 
    Column = PdbCoord[Index1 + 2];
    if(Column == 0) {
      ColumnM = 0;
    } else {
      ColumnM = Column - 1;
    } ColumnP = Column + 1;
    if(ColumnP == PdbColumnD) ColumnP = Column;
    ZDif = (Section - Z) * (Section - Z);
    ZDifM = (SectionM - Z) * (SectionM - Z);
    ZDifP = (SectionP - Z) * (SectionP - Z);
    YDif = (Row - Y) * (Row - Y);
    YDifM = (RowM - Y) * (RowM - Y);
    YDifP = (RowP - Y) * (RowP - Y);
    XDif = (Column - X) * (Column - X);
    XDifM = (ColumnM - X) * (ColumnM - X);
    XDifP = (ColumnP - X) * (ColumnP - X);
    Value = ZDif + YDif + XDif;
    if((!(
     (PdbSurface[SectionM][RowM][ColumnM]&&(ZDifM + YDifM + XDifM > Value))||
     (PdbSurface[SectionM][RowM][Column]&&(ZDifM + YDifM + XDif > Value))||
     (PdbSurface[SectionM][RowM][ColumnP]&&(ZDifM + YDifM + XDifP > Value))||
     (PdbSurface[SectionM][Row][ColumnM]&&(ZDifM + YDif + XDifM > Value))||
     (PdbSurface[SectionM][Row][Column]&&(ZDifM + YDif + XDif > Value))||
     (PdbSurface[SectionM][Row][ColumnP]&&(ZDifM + YDif + XDifP > Value))||
     (PdbSurface[SectionM][RowP][ColumnM]&&(ZDifM + YDifP + XDifM > Value))||
     (PdbSurface[SectionM][RowP][Column]&&(ZDifM + YDifP + XDif > Value))||
     (PdbSurface[SectionM][RowP][ColumnP]&&(ZDifM + YDifP + XDifP > Value))||
     (PdbSurface[Section][RowM][ColumnM]&&(ZDif + YDifM + XDifM > Value))||
     (PdbSurface[Section][RowM][Column]&&(ZDif + YDifM + XDif > Value))||
     (PdbSurface[Section][RowM][ColumnP]&&(ZDif + YDifM + XDifP > Value))||
     (PdbSurface[Section][Row][ColumnM]&&(ZDif + YDif + XDifM > Value))||
     (PdbSurface[Section][Row][ColumnP]&&(ZDif + YDif + XDifP > Value))||
     (PdbSurface[Section][RowP][ColumnM]&&(ZDif + YDifP + XDifM > Value))||
     (PdbSurface[Section][RowP][Column]&&(ZDif + YDifP + XDif > Value))||
     (PdbSurface[Section][RowP][ColumnP]&&(ZDif + YDifP + XDifP > Value))||
     (PdbSurface[SectionP][RowM][ColumnM]&&(ZDifP + YDifM + XDifM > Value))||
     (PdbSurface[SectionP][RowM][Column]&&(ZDifP + YDifM + XDif > Value))||
     (PdbSurface[SectionP][RowM][ColumnP]&&(ZDifP + YDifM + XDifP > Value))||
     (PdbSurface[SectionP][Row][ColumnM]&&(ZDifP + YDif + XDifM > Value))||
     (PdbSurface[SectionP][Row][Column]&&(ZDifP + YDif + XDif > Value))||
     (PdbSurface[SectionP][Row][ColumnP]&&(ZDifP + YDif + XDifP > Value))||
     (PdbSurface[SectionP][RowP][ColumnM]&&(ZDifP + YDifP + XDifM > Value))||
     (PdbSurface[SectionP][RowP][Column]&&(ZDifP + YDifP + XDif > Value))||
     (PdbSurface[SectionP][RowP][ColumnP]&&(ZDifP + YDifP + XDifP > Value))))
     ||(!(
     (PdbSurface[SectionM][RowM][ColumnM]&&(ZDifM + YDifM + XDifM < Value))||
     (PdbSurface[SectionM][RowM][Column]&&(ZDifM + YDifM + XDif < Value))||
     (PdbSurface[SectionM][RowM][ColumnP]&&(ZDifM + YDifM + XDifP < Value))||
     (PdbSurface[SectionM][Row][ColumnM]&&(ZDifM + YDif + XDifM < Value))||
     (PdbSurface[SectionM][Row][Column]&&(ZDifM + YDif + XDif < Value))||
     (PdbSurface[SectionM][Row][ColumnP]&&(ZDifM + YDif + XDifP < Value))||
     (PdbSurface[SectionM][RowP][ColumnM]&&(ZDifM + YDifP + XDifM < Value))||
     (PdbSurface[SectionM][RowP][Column]&&(ZDifM + YDifP + XDif < Value))||
     (PdbSurface[SectionM][RowP][ColumnP]&&(ZDifM + YDifP + XDifP < Value))||
     (PdbSurface[Section][RowM][ColumnM]&&(ZDif + YDifM + XDifM < Value))||
     (PdbSurface[Section][RowM][Column]&&(ZDif + YDifM + XDif < Value))||
     (PdbSurface[Section][RowM][ColumnP]&&(ZDif + YDifM + XDifP < Value))||
     (PdbSurface[Section][Row][ColumnM]&&(ZDif + YDif + XDifM < Value))||
     (PdbSurface[Section][Row][ColumnP]&&(ZDif + YDif + XDifP < Value))||
     (PdbSurface[Section][RowP][ColumnM]&&(ZDif + YDifP + XDifM < Value))||
     (PdbSurface[Section][RowP][Column]&&(ZDif + YDifP + XDif < Value))||
     (PdbSurface[Section][RowP][ColumnP]&&(ZDif + YDifP + XDifP < Value))||
     (PdbSurface[SectionP][RowM][ColumnM]&&(ZDifP + YDifM + XDifM < Value))||
     (PdbSurface[SectionP][RowM][Column]&&(ZDifP + YDifM + XDif < Value))||
     (PdbSurface[SectionP][RowM][ColumnP]&&(ZDifP + YDifM + XDifP < Value))||
     (PdbSurface[SectionP][Row][ColumnM]&&(ZDifP + YDif + XDifM < Value))||
     (PdbSurface[SectionP][Row][Column]&&(ZDifP + YDif + XDif < Value))||
     (PdbSurface[SectionP][Row][ColumnP]&&(ZDifP + YDif + XDifP < Value))||
     (PdbSurface[SectionP][RowP][ColumnM]&&(ZDifP + YDifP + XDifM < Value))||
     (PdbSurface[SectionP][RowP][Column]&&(ZDifP + YDifP + XDif < Value))||
     (PdbSurface[SectionP][RowP][ColumnP]&&(ZDifP + YDifP + XDifP < Value))))) {
      ExtremaCounter += 3;
      Extrema = realloc(Extrema, ExtremaCounter * sizeof(long int));
      OutOfMemory(Extrema);
      PdbVector = realloc(PdbVector, ExtremaCounter * sizeof(long int));
      OutOfMemory(PdbVector);
      Extrema[ExtremaCounter - 3] = Section;
      Extrema[ExtremaCounter - 2] = Row;
      Extrema[ExtremaCounter - 1] = Column;
      if(Section == 0) {
        if(Row == 0) {
	  if(Column == 0) {
            PdbVector[ExtremaCounter - 3] =
               PdbVolume[1][0][0]
             + PdbVolume[1][0][1]
             + PdbVolume[1][1][0]
             + PdbVolume[1][1][1];
            PdbVector[ExtremaCounter - 2] =
               PdbVolume[0][1][0]
             + PdbVolume[0][1][1]
             + PdbVolume[1][1][0]
             + PdbVolume[1][1][1];
            PdbVector[ExtremaCounter - 1] =
               PdbVolume[0][0][1]
             + PdbVolume[0][1][1]
             + PdbVolume[1][0][1]
             + PdbVolume[1][1][1];
          } else {
            if(Column == PdbColumnD - 1) {
              PdbVector[ExtremaCounter - 3] =
                 PdbVolume[1][0][ColumnM]
               + PdbVolume[1][0][Column]
               + PdbVolume[1][1][ColumnM]
	       + PdbVolume[1][1][Column];
              PdbVector[ExtremaCounter - 2] =
                 PdbVolume[0][1][ColumnM]
               + PdbVolume[0][1][Column]
               + PdbVolume[1][1][ColumnM]
	       + PdbVolume[1][1][Column];
              PdbVector[ExtremaCounter - 1] =
                -PdbVolume[0][0][ColumnM]
               - PdbVolume[0][1][ColumnM]
               - PdbVolume[1][0][ColumnM]
	       - PdbVolume[1][1][ColumnM];
	    } else {
              PdbVector[ExtremaCounter - 3] =
                 PdbVolume[1][0][ColumnM]
               + PdbVolume[1][0][Column]
               + PdbVolume[1][0][ColumnP]
               + PdbVolume[1][1][ColumnM]
               + PdbVolume[1][1][Column]
               + PdbVolume[1][1][ColumnP];
              PdbVector[ExtremaCounter - 2] =
                 PdbVolume[0][1][ColumnM]
               + PdbVolume[0][1][Column]
               + PdbVolume[0][1][ColumnP]
               + PdbVolume[1][1][ColumnM]
               + PdbVolume[1][1][Column]
               + PdbVolume[1][1][ColumnP];
              PdbVector[ExtremaCounter - 1] =
                -PdbVolume[0][0][ColumnM]
               + PdbVolume[0][0][ColumnP]
               - PdbVolume[0][1][ColumnM]
               + PdbVolume[0][1][ColumnP]
               - PdbVolume[1][0][ColumnM]
               + PdbVolume[1][0][ColumnP]
               - PdbVolume[1][1][ColumnM]
               + PdbVolume[1][1][ColumnP];
	}}} else {
	  if(Row == PdbRowD - 1) {
            if(Column == 0) {
              PdbVector[ExtremaCounter - 3] =
                 PdbVolume[1][RowM][0]
               + PdbVolume[1][RowM][1]
               + PdbVolume[1][Row][0]
	       + PdbVolume[1][Row][1];
              PdbVector[ExtremaCounter - 2] =
                -PdbVolume[0][RowM][0]
               - PdbVolume[0][RowM][1]
               - PdbVolume[1][RowM][0]
               - PdbVolume[1][RowM][1];
              PdbVector[ExtremaCounter - 1] =
                 PdbVolume[0][RowM][1]
               + PdbVolume[0][Row][1]
               + PdbVolume[1][RowM][1]
               + PdbVolume[1][Row][1];
	    } else {
              if(Column == PdbColumnD - 1) {
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][RowM][ColumnM]
                 + PdbVolume[1][RowM][Column]
                 + PdbVolume[1][Row][ColumnM]
      	         + PdbVolume[1][Row][Column];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[0][RowM][ColumnM]
                 - PdbVolume[0][RowM][Column]
                 - PdbVolume[1][RowM][ColumnM]
	         - PdbVolume[1][RowM][Column];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[0][RowM][ColumnM]
                 - PdbVolume[0][Row][ColumnM]
                 - PdbVolume[1][RowM][ColumnM]
        	 - PdbVolume[1][Row][ColumnM];
              } else {
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][RowM][ColumnM]
                 + PdbVolume[1][RowM][Column]
                 + PdbVolume[1][RowM][ColumnP]
                 + PdbVolume[1][Row][ColumnM]
                 + PdbVolume[1][Row][Column]
      	         + PdbVolume[1][Row][ColumnP];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[0][RowM][ColumnM]
                 - PdbVolume[0][RowM][Column]
                 - PdbVolume[0][RowM][ColumnP]
                 - PdbVolume[1][RowM][ColumnM]
                 - PdbVolume[1][RowM][Column]
        	 - PdbVolume[1][RowM][ColumnP];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[0][RowM][ColumnM]
                 + PdbVolume[0][RowM][ColumnP]
                 - PdbVolume[0][Row][ColumnM]
                 + PdbVolume[0][Row][ColumnP]
                 - PdbVolume[1][RowM][ColumnM]
                 + PdbVolume[1][RowM][ColumnP]
                 - PdbVolume[1][Row][ColumnM]
        	 + PdbVolume[1][Row][ColumnP];
	  }}} else {
	    if(Column == 0) {    
              PdbVector[ExtremaCounter - 3] =
                 PdbVolume[1][RowM][0]
               + PdbVolume[1][RowM][1]
               + PdbVolume[1][Row][0]
               + PdbVolume[1][Row][1]
               + PdbVolume[1][RowP][0]
               + PdbVolume[1][RowP][1];
              PdbVector[ExtremaCounter - 2] =
                -PdbVolume[0][RowM][0]
               - PdbVolume[0][RowM][1]
               + PdbVolume[0][RowP][0]
               + PdbVolume[0][RowP][1]
               - PdbVolume[1][RowM][0]
               - PdbVolume[1][RowM][1]
               + PdbVolume[1][RowP][0]
               + PdbVolume[1][RowP][1];
              PdbVector[ExtremaCounter - 1] =
                +PdbVolume[0][RowM][1]
               + PdbVolume[0][Row][1]
               + PdbVolume[0][RowP][1]
               + PdbVolume[1][RowM][1]
               + PdbVolume[1][Row][1]
               + PdbVolume[1][RowP][1];
            } else {
              if(Column = PdbColumnD - 1) {
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][RowM][ColumnM]
                 + PdbVolume[1][RowM][Column]
                 + PdbVolume[1][Row][ColumnM]
                 + PdbVolume[1][Row][Column]
                 + PdbVolume[1][RowP][ColumnM]
      	         + PdbVolume[1][RowP][Column];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[0][RowM][ColumnM]
                 - PdbVolume[0][RowM][Column]
                 + PdbVolume[0][RowP][ColumnM]
                 + PdbVolume[0][RowP][Column]
                 - PdbVolume[1][RowM][ColumnM]
                 - PdbVolume[1][RowM][Column]
                 + PdbVolume[1][RowP][ColumnM]
         	 + PdbVolume[1][RowP][Column];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[0][RowM][ColumnM]
                 - PdbVolume[0][Row][ColumnM]
                 - PdbVolume[0][RowP][ColumnM]
                 - PdbVolume[1][RowM][ColumnM]
	         - PdbVolume[1][Row][ColumnM]
	         - PdbVolume[1][RowP][ColumnM];
	      } else {
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][RowM][ColumnM]
                 + PdbVolume[1][RowM][Column]
                 + PdbVolume[1][RowM][ColumnP]
                 + PdbVolume[1][Row][ColumnM]
                 + PdbVolume[1][Row][Column]
                 + PdbVolume[1][Row][ColumnP]
                 + PdbVolume[1][RowP][ColumnM]
                 + PdbVolume[1][RowP][Column]
                 + PdbVolume[1][RowP][ColumnP];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[0][RowM][ColumnM]
                 - PdbVolume[0][RowM][Column]
                 - PdbVolume[0][RowM][ColumnP]
                 + PdbVolume[0][RowP][ColumnM]
                 + PdbVolume[0][RowP][Column]
                 + PdbVolume[0][RowP][ColumnP]
                 - PdbVolume[1][RowM][ColumnM]
                 - PdbVolume[1][RowM][Column]
                 - PdbVolume[1][RowM][ColumnP]
                 + PdbVolume[1][RowP][ColumnM]
                 + PdbVolume[1][RowP][Column]
                 + PdbVolume[1][RowP][ColumnP];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[0][RowM][ColumnM]
                 + PdbVolume[0][RowM][ColumnP]
                 - PdbVolume[0][Row][ColumnM]
                 + PdbVolume[0][Row][ColumnP]
                 - PdbVolume[0][RowP][ColumnM]
                 + PdbVolume[0][RowP][ColumnP]
                 - PdbVolume[1][RowM][ColumnM]
                 + PdbVolume[1][RowM][ColumnP]
                 - PdbVolume[1][Row][ColumnM]
                 + PdbVolume[1][Row][ColumnP]
                 - PdbVolume[1][RowP][ColumnM]
                 + PdbVolume[1][RowP][ColumnP];
      }}}}} else {
        if(Section == PdbSectionD - 1) {
          if(Row == 0) {
            if(Column == 0) {
              PdbVector[ExtremaCounter - 3] =
                -PdbVolume[SectionM][0][0]
               - PdbVolume[SectionM][0][1]
               - PdbVolume[SectionM][1][0]
               - PdbVolume[SectionM][1][1];
              PdbVector[ExtremaCounter - 2] =
                 PdbVolume[SectionM][1][0]
               + PdbVolume[SectionM][1][1]
               + PdbVolume[Section][1][0]
 	       + PdbVolume[Section][1][1];
              PdbVector[ExtremaCounter - 1] =
                 PdbVolume[SectionM][0][1]
               + PdbVolume[SectionM][1][1]
               + PdbVolume[Section][0][1]
               + PdbVolume[Section][1][1];
	    } else {
              if(Column == PdbColumnD - 1) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][0][ColumnM]
                 - PdbVolume[SectionM][0][Column]
                 - PdbVolume[SectionM][1][ColumnM]
	         - PdbVolume[SectionM][1][Column];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[SectionM][1][ColumnM]
                 + PdbVolume[SectionM][1][Column]
                 + PdbVolume[Section][1][ColumnM]
                 + PdbVolume[Section][1][Column];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[SectionM][0][ColumnM]
                 - PdbVolume[SectionM][1][ColumnM]
                 - PdbVolume[Section][0][ColumnM]
      	         - PdbVolume[Section][1][ColumnM];
	      } else {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][0][ColumnM]
                 - PdbVolume[SectionM][0][Column]
                 - PdbVolume[SectionM][0][ColumnP]
                 - PdbVolume[SectionM][1][ColumnM]
                 - PdbVolume[SectionM][1][Column]
                 - PdbVolume[SectionM][1][ColumnP];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[SectionM][1][ColumnM]
                 + PdbVolume[SectionM][1][Column]
                 + PdbVolume[SectionM][1][ColumnP]
                 + PdbVolume[Section][1][ColumnM]
                 + PdbVolume[Section][1][Column]
                 + PdbVolume[Section][1][ColumnP];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[SectionM][0][ColumnM]
                 + PdbVolume[SectionM][0][ColumnP]
                 - PdbVolume[SectionM][1][ColumnM]
                 + PdbVolume[SectionM][1][ColumnP]
                 - PdbVolume[Section][0][ColumnM]
                 + PdbVolume[Section][0][ColumnP]
                 - PdbVolume[Section][1][ColumnM]
                 + PdbVolume[Section][1][ColumnP];
	  }}} else {
            if(Row == PdbRowD - 1) {
              if(Column == 0) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 - PdbVolume[SectionM][Row][0]
	         - PdbVolume[SectionM][Row][1];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 - PdbVolume[Section][RowM][0]
       	         - PdbVolume[Section][RowM][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[SectionM][RowM][1]
                 + PdbVolume[SectionM][Row][1]
                 + PdbVolume[Section][RowM][1]
         	 + PdbVolume[Section][Row][1];
	      } else {
	        if(Column == PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][Row][ColumnM]
		   - PdbVolume[SectionM][Row][Column];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[Section][RowM][ColumnM]
		   - PdbVolume[Section][RowM][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[Section][RowM][ColumnM]
		   - PdbVolume[Section][Row][ColumnM];
		} else {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
		   - PdbVolume[SectionM][Row][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
		   - PdbVolume[Section][RowM][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   + PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   + PdbVolume[SectionM][Row][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   + PdbVolume[Section][RowM][ColumnP]
                   - PdbVolume[Section][Row][ColumnM]
		   + PdbVolume[Section][Row][ColumnP];
	    }}} else {
              if(Column == 0) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 - PdbVolume[SectionM][Row][0]
                 - PdbVolume[SectionM][Row][1]
                 - PdbVolume[SectionM][RowP][0]
                 - PdbVolume[SectionM][RowP][1];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 + PdbVolume[SectionM][RowP][0]
                 + PdbVolume[SectionM][RowP][1]
                 - PdbVolume[Section][RowM][0]
                 - PdbVolume[Section][RowM][1]
                 + PdbVolume[Section][RowP][0]
                 + PdbVolume[Section][RowP][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[SectionM][RowM][1]
                 + PdbVolume[SectionM][Row][1]
                 + PdbVolume[SectionM][RowP][1]
                 + PdbVolume[Section][RowM][1]
                 + PdbVolume[Section][Row][1]
                 + PdbVolume[Section][RowP][1];
	      } else {	
                if(Column == PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   - PdbVolume[SectionM][RowP][Column];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   + PdbVolume[SectionM][RowP][ColumnM]
                   + PdbVolume[SectionM][RowP][Column]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
                   + PdbVolume[Section][RowP][ColumnM]
                   + PdbVolume[Section][RowP][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][Row][ColumnM]
                   - PdbVolume[Section][RowP][ColumnM];
		} else {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
                   - PdbVolume[SectionM][Row][ColumnP]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   - PdbVolume[SectionM][RowP][Column]
                   - PdbVolume[SectionM][RowP][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   + PdbVolume[SectionM][RowP][ColumnM]
                   + PdbVolume[SectionM][RowP][Column]
                   + PdbVolume[SectionM][RowP][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
                   - PdbVolume[Section][RowM][ColumnP]
                   + PdbVolume[Section][RowP][ColumnM]
                   + PdbVolume[Section][RowP][Column]
 	           + PdbVolume[Section][RowP][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   + PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   + PdbVolume[SectionM][Row][ColumnP]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   + PdbVolume[SectionM][RowP][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   + PdbVolume[Section][RowM][ColumnP]
                   - PdbVolume[Section][Row][ColumnM]
                   + PdbVolume[Section][Row][ColumnP]
                   - PdbVolume[Section][RowP][ColumnM]
                   + PdbVolume[Section][RowP][ColumnP];
	}}}}} else {
          if(Row == 0) {
            if(Column == 0) {
              PdbVector[ExtremaCounter - 3] =
                -PdbVolume[SectionM][0][0]
               - PdbVolume[SectionM][0][1]
               - PdbVolume[SectionM][1][0]
               - PdbVolume[SectionM][1][1]
               + PdbVolume[SectionP][0][0]
               + PdbVolume[SectionP][0][1]
               + PdbVolume[SectionP][1][0]
               + PdbVolume[SectionP][1][1];
              PdbVector[ExtremaCounter - 2] =
                 PdbVolume[SectionM][1][0]
               + PdbVolume[SectionM][1][1]
               + PdbVolume[Section][1][0]
               + PdbVolume[Section][1][1]
               + PdbVolume[SectionP][1][0]
               + PdbVolume[SectionP][1][1];
              PdbVector[ExtremaCounter - 1] =
                 PdbVolume[SectionM][0][1]
               + PdbVolume[SectionM][1][1]
               + PdbVolume[Section][0][1]
               + PdbVolume[Section][1][1]
               + PdbVolume[SectionP][0][1]
               + PdbVolume[SectionP][1][1];
	    } else {
              if(Column == PdbColumnD - 1) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][0][ColumnM]
                 - PdbVolume[SectionM][0][Column]
                 - PdbVolume[SectionM][1][ColumnM]
                 - PdbVolume[SectionM][1][Column]
                 + PdbVolume[SectionP][0][ColumnM]
                 + PdbVolume[SectionP][0][Column]
                 + PdbVolume[SectionP][1][ColumnM]
                 + PdbVolume[SectionP][1][Column];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[SectionM][1][ColumnM]
                 + PdbVolume[SectionM][1][Column]
                 + PdbVolume[Section][1][ColumnM]
                 + PdbVolume[Section][1][Column]
                 + PdbVolume[SectionP][1][ColumnM]
                 + PdbVolume[SectionP][1][Column];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[SectionM][0][ColumnM]
                 - PdbVolume[SectionM][1][ColumnM]
                 - PdbVolume[Section][0][ColumnM]
                 - PdbVolume[Section][1][ColumnM]
                 - PdbVolume[SectionP][0][ColumnM]
                 - PdbVolume[SectionP][1][ColumnM];
              } else {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][0][ColumnM]
                 - PdbVolume[SectionM][0][Column]
                 - PdbVolume[SectionM][0][ColumnP]
                 - PdbVolume[SectionM][1][ColumnM]
                 - PdbVolume[SectionM][1][Column]
                 - PdbVolume[SectionM][1][ColumnP]
                 + PdbVolume[SectionP][0][ColumnM]
                 + PdbVolume[SectionP][0][Column]
                 + PdbVolume[SectionP][0][ColumnP]
                 + PdbVolume[SectionP][1][ColumnM]
                 + PdbVolume[SectionP][1][Column]
                 + PdbVolume[SectionP][1][ColumnP];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[SectionM][1][ColumnM]
                 + PdbVolume[SectionM][1][Column]
                 + PdbVolume[SectionM][1][ColumnP]
                 + PdbVolume[Section][1][ColumnM]
                 + PdbVolume[Section][1][Column]
                 + PdbVolume[Section][1][ColumnP]
                 + PdbVolume[SectionP][1][ColumnM]
                 + PdbVolume[SectionP][1][Column]
                 + PdbVolume[SectionP][1][ColumnP];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[SectionM][0][ColumnM]
                 + PdbVolume[SectionM][0][ColumnP]
                 - PdbVolume[SectionM][1][ColumnM]
                 + PdbVolume[SectionM][1][ColumnP]
                 - PdbVolume[Section][0][ColumnM]
                 + PdbVolume[Section][0][ColumnP]
                 - PdbVolume[Section][1][ColumnM]
                 + PdbVolume[Section][1][ColumnP]
                 - PdbVolume[SectionP][0][ColumnM]
                 + PdbVolume[SectionP][0][ColumnP]
                 - PdbVolume[SectionP][1][ColumnM]
                 + PdbVolume[SectionP][1][ColumnP];
          }}} else {
	    if(Row == PdbRowD - 1) {
              if(Column == 0) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 - PdbVolume[SectionM][Row][0]
                 - PdbVolume[SectionM][Row][1]
                 + PdbVolume[SectionP][RowM][0]
                 + PdbVolume[SectionP][RowM][1]
                 + PdbVolume[SectionP][Row][0]
                 + PdbVolume[SectionP][Row][1];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 - PdbVolume[Section][RowM][0]
                 - PdbVolume[Section][RowM][1]
                 - PdbVolume[SectionP][RowM][0]
                 - PdbVolume[SectionP][RowM][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[SectionM][RowM][1]
                 + PdbVolume[SectionM][Row][1]
                 + PdbVolume[Section][RowM][1]
                 + PdbVolume[Section][Row][1]
                 + PdbVolume[SectionP][RowM][1]
                 + PdbVolume[SectionP][Row][1];
              } else {
                if(Column == PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
                   + PdbVolume[SectionP][RowM][ColumnM]
                   + PdbVolume[SectionP][RowM][Column]
                   + PdbVolume[SectionP][Row][ColumnM]
                   + PdbVolume[SectionP][Row][Column];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   - PdbVolume[SectionP][RowM][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][Row][ColumnM]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   - PdbVolume[SectionP][Row][ColumnM];
                } else {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
                   - PdbVolume[SectionM][Row][ColumnP]
                   + PdbVolume[SectionP][RowM][ColumnM]
                   + PdbVolume[SectionP][RowM][Column]
                   + PdbVolume[SectionP][RowM][ColumnP]
                   + PdbVolume[SectionP][Row][ColumnM]
                   + PdbVolume[SectionP][Row][Column]
                   + PdbVolume[SectionP][Row][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
                   - PdbVolume[Section][RowM][ColumnP]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   - PdbVolume[SectionP][RowM][Column]
                   - PdbVolume[SectionP][RowM][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   + PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   + PdbVolume[SectionM][Row][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   + PdbVolume[Section][RowM][ColumnP]
                   - PdbVolume[Section][Row][ColumnM]
                   + PdbVolume[Section][Row][ColumnP]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   + PdbVolume[SectionP][RowM][ColumnP]
                   - PdbVolume[SectionP][Row][ColumnM]
                   + PdbVolume[SectionP][Row][ColumnP];
            }}} else {
              if(Column == 0) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 - PdbVolume[SectionM][Row][0]
                 - PdbVolume[SectionM][Row][1]
                 - PdbVolume[SectionM][RowP][0]
                 - PdbVolume[SectionM][RowP][1]
                 + PdbVolume[SectionP][RowM][0]
                 + PdbVolume[SectionP][RowM][1]
                 + PdbVolume[SectionP][Row][0]
                 + PdbVolume[SectionP][Row][1]
                 + PdbVolume[SectionP][RowP][0]
                 + PdbVolume[SectionP][RowP][1];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[SectionM][RowM][0]
                 - PdbVolume[SectionM][RowM][1]
                 + PdbVolume[SectionM][RowP][0]
                 + PdbVolume[SectionM][RowP][1]
                 - PdbVolume[Section][RowM][0]
                 - PdbVolume[Section][RowM][1]
                 + PdbVolume[Section][RowP][0]
                 + PdbVolume[Section][RowP][1]
                 - PdbVolume[SectionP][RowM][0]
                 - PdbVolume[SectionP][RowM][1]
                 + PdbVolume[SectionP][RowP][0]
                 + PdbVolume[SectionP][RowP][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[SectionM][RowM][1]
                 + PdbVolume[SectionM][Row][1]
                 + PdbVolume[SectionM][RowP][1]
                 + PdbVolume[Section][RowM][1]
                 + PdbVolume[Section][Row][1]
                 + PdbVolume[Section][RowP][1]
                 + PdbVolume[SectionP][RowM][1]
                 + PdbVolume[SectionP][Row][1]
                 + PdbVolume[SectionP][RowP][1];
              } else {
	        if(Column == PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   - PdbVolume[SectionM][RowP][Column]
                   + PdbVolume[SectionP][RowM][ColumnM]
                   + PdbVolume[SectionP][RowM][Column]
                   + PdbVolume[SectionP][Row][ColumnM]
                   + PdbVolume[SectionP][Row][Column]
                   + PdbVolume[SectionP][RowP][ColumnM]
                   + PdbVolume[SectionP][RowP][Column];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   + PdbVolume[SectionM][RowP][ColumnM]
                   + PdbVolume[SectionM][RowP][Column]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
                   + PdbVolume[Section][RowP][ColumnM]
                   + PdbVolume[Section][RowP][Column]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   - PdbVolume[SectionP][RowM][Column]
                   + PdbVolume[SectionP][RowP][ColumnM]
                   + PdbVolume[SectionP][RowP][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][Row][ColumnM]
                   - PdbVolume[Section][RowP][ColumnM]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   - PdbVolume[SectionP][Row][ColumnM]
                   - PdbVolume[SectionP][RowP][ColumnM];
                } else {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
                   - PdbVolume[SectionM][Row][ColumnP]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   - PdbVolume[SectionM][RowP][Column]
                   - PdbVolume[SectionM][RowP][ColumnP]
                   + PdbVolume[SectionP][RowM][ColumnM]
                   + PdbVolume[SectionP][RowM][Column]
                   + PdbVolume[SectionP][RowM][ColumnP]
                   + PdbVolume[SectionP][Row][ColumnM]
                   + PdbVolume[SectionP][Row][Column]
                   + PdbVolume[SectionP][Row][ColumnP]
                   + PdbVolume[SectionP][RowP][ColumnM]
                   + PdbVolume[SectionP][RowP][Column]
                   + PdbVolume[SectionP][RowP][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   + PdbVolume[SectionM][RowP][ColumnM]
                   + PdbVolume[SectionM][RowP][Column]
                   + PdbVolume[SectionM][RowP][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
                   - PdbVolume[Section][RowM][ColumnP]
                   + PdbVolume[Section][RowP][ColumnM]
                   + PdbVolume[Section][RowP][Column]
                   + PdbVolume[Section][RowP][ColumnP]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   - PdbVolume[SectionP][RowM][Column]
                   - PdbVolume[SectionP][RowM][ColumnP]
                   + PdbVolume[SectionP][RowP][ColumnM]
                   + PdbVolume[SectionP][RowP][Column]
                   + PdbVolume[SectionP][RowP][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   + PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   + PdbVolume[SectionM][Row][ColumnP]
                   - PdbVolume[SectionM][RowP][ColumnM]
                   + PdbVolume[SectionM][RowP][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   + PdbVolume[Section][RowM][ColumnP]
                   - PdbVolume[Section][Row][ColumnM]
                   + PdbVolume[Section][Row][ColumnP]
                   - PdbVolume[Section][RowP][ColumnM]
                   + PdbVolume[Section][RowP][ColumnP]
                   - PdbVolume[SectionP][RowM][ColumnM]
                   + PdbVolume[SectionP][RowM][ColumnP]
                   - PdbVolume[SectionP][Row][ColumnM]
                   + PdbVolume[SectionP][Row][ColumnP]
                   - PdbVolume[SectionP][RowP][ColumnM]
                   + PdbVolume[SectionP][RowP][ColumnP];
  }}}}}}}} if(ExtremaCounter < 120) {
    for(Index1 = 0; Index1 < PdbCoordCounter; Index1 += 3) {
      Section = PdbCoord[Index1];
      if(Section == 0) {
        SectionM = 0;
      } else {
        SectionM = Section - 1;
      } SectionP = Section + 1;
      if(SectionP == PdbSectionD) SectionP = Section;
      Row = PdbCoord[Index1 + 1];
      if(Row == 0) {
        RowM = 0;
      } else {
        RowM = Row - 1;
      } RowP = Row + 1;
      if(RowP == PdbRowD) RowP = Row; 
      Column = PdbCoord[Index1 + 2];
      if(Column == 0) {
        ColumnM = 0;
      } else {
        ColumnM = Column - 1;
      } ColumnP = Column + 1;
      if(ColumnP == PdbColumnD) ColumnP = Column;
      ZDif = (Section - Z) * (Section - Z);
      ZDifM = (SectionM - Z) * (SectionM - Z);
      ZDifP = (SectionP - Z) * (SectionP - Z);
      YDif = (Row - Y) * (Row - Y);
      YDifM = (RowM - Y) * (RowM - Y);
      YDifP = (RowP - Y) * (RowP - Y);
      XDif = (Column - X) * (Column - X);
      XDifM = (ColumnM - X) * (ColumnM - X);
      XDifP = (ColumnP - X) * (ColumnP - X);
      Value = ZDif + YDif + XDif;
      Index2 = 0; /* number of more distant neighbours */
      Index3 = 0; /* number of less or equally distant neighbours */
      if(PdbSurface[SectionM][RowM][ColumnM]) {
        if(ZDifM + YDifM + XDifM > Value) {
          ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionM][RowM][Column]) {
	if(ZDifM + YDifM + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionM][RowM][ColumnP]) {
	if(ZDifM + YDifM + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionM][Row][ColumnM]) {
	if(ZDifM + YDif + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionM][Row][Column]) {
	if(ZDifM + YDif + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionM][Row][ColumnP]) {
        if(ZDifM + YDif + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }}if(PdbSurface[SectionM][RowP][ColumnM]) {
	if(ZDifM + YDifP + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionM][RowP][Column]) {
	if(ZDifM + YDifP + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionM][RowP][ColumnP]) {
	if(ZDifM + YDifP + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][RowM][ColumnM]) {
	if(ZDif + YDifM + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][RowM][Column]) {
	if(ZDif + YDifM + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][RowM][ColumnP]) {
	if(ZDif + YDifM + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][Row][ColumnM]) {
	if(ZDif + YDif + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][Row][ColumnP]) {
	if(ZDif + YDif + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][RowP][ColumnM]) {
	if(ZDif + YDifP + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][RowP][Column]) {
	if(ZDif + YDifP + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[Section][RowP][ColumnP]) {
	if(ZDif + YDifP + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][RowM][ColumnM]) {
	if(ZDifP + YDifM + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][RowM][Column]) {
	if(ZDifP + YDifM + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][RowM][ColumnP]) {
	if(ZDifP + YDifM + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][Row][ColumnM]) {
	if(ZDifP + YDif + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][Row][Column]) {
        if(ZDifP + YDif + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][Row][ColumnP]) {
        if(ZDifP + YDif + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][RowP][ColumnM]) {
        if(ZDifP + YDifP + XDifM > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][RowP][Column]) {
	if(ZDifP + YDifP + XDif > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(PdbSurface[SectionP][RowP][ColumnP]) {
	if(ZDifP + YDifP + XDifP > Value) {
	  ++Index2;
	} else {
	  ++Index3;
      }} if(Index2 == 2 && Index3 == 6) {
        ExtremaCounter += 3;
        Extrema = realloc(Extrema, ExtremaCounter * sizeof(long int));
        OutOfMemory(Extrema);
        PdbVector = realloc(PdbVector, ExtremaCounter * sizeof(long int));
        OutOfMemory(PdbVector);
        Extrema[ExtremaCounter - 3] = Section;
        Extrema[ExtremaCounter - 2] = Row;
        Extrema[ExtremaCounter - 1] = Column;
        if(Section == 0) {
          if(Row == 0) {
	    if(Column == 0) {
              PdbVector[ExtremaCounter - 3] =
                 PdbVolume[1][0][0]
               + PdbVolume[1][0][1]
               + PdbVolume[1][1][0]
               + PdbVolume[1][1][1];
              PdbVector[ExtremaCounter - 2] =
                 PdbVolume[0][1][0]
               + PdbVolume[0][1][1]
               + PdbVolume[1][1][0]
               + PdbVolume[1][1][1];
              PdbVector[ExtremaCounter - 1] =
                 PdbVolume[0][0][1]
               + PdbVolume[0][1][1]
               + PdbVolume[1][0][1]
               + PdbVolume[1][1][1];
            } else {
              if(Column == PdbColumnD - 1) {
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][0][ColumnM]
                 + PdbVolume[1][0][Column]
                 + PdbVolume[1][1][ColumnM]
	         + PdbVolume[1][1][Column];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[0][1][ColumnM]
                 + PdbVolume[0][1][Column]
                 + PdbVolume[1][1][ColumnM]
	         + PdbVolume[1][1][Column];
                PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[0][0][ColumnM]
                 - PdbVolume[0][1][ColumnM]
                 - PdbVolume[1][0][ColumnM]
	         - PdbVolume[1][1][ColumnM];
	      } else {
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][0][ColumnM]
                 + PdbVolume[1][0][Column]
                 + PdbVolume[1][0][ColumnP]
                 + PdbVolume[1][1][ColumnM]
                 + PdbVolume[1][1][Column]
                 + PdbVolume[1][1][ColumnP];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[0][1][ColumnM]
                 + PdbVolume[0][1][Column]
                 + PdbVolume[0][1][ColumnP]
                 + PdbVolume[1][1][ColumnM]
                 + PdbVolume[1][1][Column]
                 + PdbVolume[1][1][ColumnP];
              PdbVector[ExtremaCounter - 1] =
                  -PdbVolume[0][0][ColumnM]
                 + PdbVolume[0][0][ColumnP]
                 - PdbVolume[0][1][ColumnM]
                 + PdbVolume[0][1][ColumnP]
                 - PdbVolume[1][0][ColumnM]
                 + PdbVolume[1][0][ColumnP]
                 - PdbVolume[1][1][ColumnM]
                 + PdbVolume[1][1][ColumnP];
	  }}} else {
	    if(Row == PdbRowD - 1) {
              if(Column == 0) {
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][RowM][0]
                 + PdbVolume[1][RowM][1]
                 + PdbVolume[1][Row][0]
	         + PdbVolume[1][Row][1];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[0][RowM][0]
                 - PdbVolume[0][RowM][1]
                 - PdbVolume[1][RowM][0]
                 - PdbVolume[1][RowM][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[0][RowM][1]
                 + PdbVolume[0][Row][1]
                 + PdbVolume[1][RowM][1]
                 + PdbVolume[1][Row][1];
	      } else {
                if(Column == PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                     PdbVolume[1][RowM][ColumnM]
                   + PdbVolume[1][RowM][Column]
                   + PdbVolume[1][Row][ColumnM]
      	           + PdbVolume[1][Row][Column];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[0][RowM][ColumnM]
                   - PdbVolume[0][RowM][Column]
                   - PdbVolume[1][RowM][ColumnM]
	           - PdbVolume[1][RowM][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[0][RowM][ColumnM]
                   - PdbVolume[0][Row][ColumnM]
                   - PdbVolume[1][RowM][ColumnM]
        	   - PdbVolume[1][Row][ColumnM];
                } else {
                  PdbVector[ExtremaCounter - 3] =
                     PdbVolume[1][RowM][ColumnM]
                   + PdbVolume[1][RowM][Column]
                   + PdbVolume[1][RowM][ColumnP]
                   + PdbVolume[1][Row][ColumnM]
                   + PdbVolume[1][Row][Column]
      	           + PdbVolume[1][Row][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[0][RowM][ColumnM]
                   - PdbVolume[0][RowM][Column]
                   - PdbVolume[0][RowM][ColumnP]
                   - PdbVolume[1][RowM][ColumnM]
                   - PdbVolume[1][RowM][Column]
        	   - PdbVolume[1][RowM][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[0][RowM][ColumnM]
                   + PdbVolume[0][RowM][ColumnP]
                   - PdbVolume[0][Row][ColumnM]
                   + PdbVolume[0][Row][ColumnP]
                   - PdbVolume[1][RowM][ColumnM]
                   + PdbVolume[1][RowM][ColumnP]
                   - PdbVolume[1][Row][ColumnM]
        	   + PdbVolume[1][Row][ColumnP];
	    }}} else {
	      if(Column == 0) {    
                PdbVector[ExtremaCounter - 3] =
                   PdbVolume[1][RowM][0]
                 + PdbVolume[1][RowM][1]
                 + PdbVolume[1][Row][0]
                 + PdbVolume[1][Row][1]
                 + PdbVolume[1][RowP][0]
                 + PdbVolume[1][RowP][1];
                PdbVector[ExtremaCounter - 2] =
                  -PdbVolume[0][RowM][0]
                 - PdbVolume[0][RowM][1]
                 + PdbVolume[0][RowP][0]
                 + PdbVolume[0][RowP][1]
                 - PdbVolume[1][RowM][0]
                 - PdbVolume[1][RowM][1]
                 + PdbVolume[1][RowP][0]
                 + PdbVolume[1][RowP][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[0][RowM][1]
                 + PdbVolume[0][Row][1]
                 + PdbVolume[0][RowP][1]
                 + PdbVolume[1][RowM][1]
                 + PdbVolume[1][Row][1]
                 + PdbVolume[1][RowP][1];
              } else {
                if(Column = PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                     PdbVolume[1][RowM][ColumnM]
                   + PdbVolume[1][RowM][Column]
                   + PdbVolume[1][Row][ColumnM]
                   + PdbVolume[1][Row][Column]
                   + PdbVolume[1][RowP][ColumnM]
      	           + PdbVolume[1][RowP][Column];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[0][RowM][ColumnM]
                   - PdbVolume[0][RowM][Column]
                   + PdbVolume[0][RowP][ColumnM]
                   + PdbVolume[0][RowP][Column]
                   - PdbVolume[1][RowM][ColumnM]
                   - PdbVolume[1][RowM][Column]
                   + PdbVolume[1][RowP][ColumnM]
         	   + PdbVolume[1][RowP][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[0][RowM][ColumnM]
                   - PdbVolume[0][Row][ColumnM]
                   - PdbVolume[0][RowP][ColumnM]
                   - PdbVolume[1][RowM][ColumnM]
	           - PdbVolume[1][Row][ColumnM]
	           - PdbVolume[1][RowP][ColumnM];
	        } else {
                  PdbVector[ExtremaCounter - 3] =
                     PdbVolume[1][RowM][ColumnM]
                   + PdbVolume[1][RowM][Column]
                   + PdbVolume[1][RowM][ColumnP]
                   + PdbVolume[1][Row][ColumnM]
                   + PdbVolume[1][Row][Column]
                   + PdbVolume[1][Row][ColumnP]
                   + PdbVolume[1][RowP][ColumnM]
                   + PdbVolume[1][RowP][Column]
                   + PdbVolume[1][RowP][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[0][RowM][ColumnM]
                   - PdbVolume[0][RowM][Column]
                   - PdbVolume[0][RowM][ColumnP]
                   + PdbVolume[0][RowP][ColumnM]
                   + PdbVolume[0][RowP][Column]
                   + PdbVolume[0][RowP][ColumnP]
                   - PdbVolume[1][RowM][ColumnM]
                   - PdbVolume[1][RowM][Column]
                   - PdbVolume[1][RowM][ColumnP]
                   + PdbVolume[1][RowP][ColumnM]
                   + PdbVolume[1][RowP][Column]
                   + PdbVolume[1][RowP][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[0][RowM][ColumnM]
                   + PdbVolume[0][RowM][ColumnP]
                   - PdbVolume[0][Row][ColumnM]
                   + PdbVolume[0][Row][ColumnP]
                   - PdbVolume[0][RowP][ColumnM]
                   + PdbVolume[0][RowP][ColumnP]
                   - PdbVolume[1][RowM][ColumnM]
                   + PdbVolume[1][RowM][ColumnP]
                   - PdbVolume[1][Row][ColumnM]
                   + PdbVolume[1][Row][ColumnP]
                   - PdbVolume[1][RowP][ColumnM]
                   + PdbVolume[1][RowP][ColumnP];
        }}}}} else {
          if(Section == PdbSectionD - 1) {
            if(Row == 0) {
              if(Column == 0) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][0][0]
                 - PdbVolume[SectionM][0][1]
                 - PdbVolume[SectionM][1][0]
                 - PdbVolume[SectionM][1][1];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[SectionM][1][0]
                 + PdbVolume[SectionM][1][1]
                 + PdbVolume[Section][1][0]
 	         + PdbVolume[Section][1][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[SectionM][0][1]
                 + PdbVolume[SectionM][1][1]
                 + PdbVolume[Section][0][1]
                 + PdbVolume[Section][1][1];
	      } else {
                if(Column == PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][0][ColumnM]
                   - PdbVolume[SectionM][0][Column]
                   - PdbVolume[SectionM][1][ColumnM]
	           - PdbVolume[SectionM][1][Column];
                  PdbVector[ExtremaCounter - 2] =
                     PdbVolume[SectionM][1][ColumnM]
                   + PdbVolume[SectionM][1][Column]
                   + PdbVolume[Section][1][ColumnM]
                   + PdbVolume[Section][1][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][0][ColumnM]
                   - PdbVolume[SectionM][1][ColumnM]
                   - PdbVolume[Section][0][ColumnM]
      	           - PdbVolume[Section][1][ColumnM];
	        } else {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][0][ColumnM]
                   - PdbVolume[SectionM][0][Column]
                   - PdbVolume[SectionM][0][ColumnP]
                   - PdbVolume[SectionM][1][ColumnM]
                   - PdbVolume[SectionM][1][Column]
                   - PdbVolume[SectionM][1][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                     PdbVolume[SectionM][1][ColumnM]
                   + PdbVolume[SectionM][1][Column]
                   + PdbVolume[SectionM][1][ColumnP]
                   + PdbVolume[Section][1][ColumnM]
                   + PdbVolume[Section][1][Column]
                   + PdbVolume[Section][1][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                     PdbVolume[SectionM][0][ColumnM]
                   + PdbVolume[SectionM][0][ColumnP]
                   - PdbVolume[SectionM][1][ColumnM]
                   + PdbVolume[SectionM][1][ColumnP]
                   - PdbVolume[Section][0][ColumnM]
                   + PdbVolume[Section][0][ColumnP]
                   - PdbVolume[Section][1][ColumnM]
                   + PdbVolume[Section][1][ColumnP];
	    }}} else {
              if(Row == PdbRowD - 1) {
                if(Column == 0) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   - PdbVolume[SectionM][Row][0]
	           - PdbVolume[SectionM][Row][1];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   - PdbVolume[Section][RowM][0]
       	           - PdbVolume[Section][RowM][1];
                  PdbVector[ExtremaCounter - 1] =
                     PdbVolume[SectionM][RowM][1]
                   + PdbVolume[SectionM][Row][1]
                   + PdbVolume[Section][RowM][1]
         	   + PdbVolume[Section][Row][1];
	        } else {
	          if(Column == PdbColumnD - 1) {
                    PdbVector[ExtremaCounter - 3] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][Row][ColumnM]
		     - PdbVolume[SectionM][Row][Column];
                    PdbVector[ExtremaCounter - 2] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[Section][RowM][ColumnM]
		     - PdbVolume[Section][RowM][Column];
                   PdbVector[ExtremaCounter - 1] =
                     -PdbVolume[SectionM][RowM][ColumnM]
                    - PdbVolume[SectionM][Row][ColumnM]
                    - PdbVolume[Section][RowM][ColumnM]
		    - PdbVolume[Section][Row][ColumnM];
		 } else {
                   PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   - PdbVolume[SectionM][Row][Column]
		   - PdbVolume[SectionM][Row][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   - PdbVolume[SectionM][RowM][Column]
                   - PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   - PdbVolume[Section][RowM][Column]
		   - PdbVolume[Section][RowM][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][RowM][ColumnM]
                   + PdbVolume[SectionM][RowM][ColumnP]
                   - PdbVolume[SectionM][Row][ColumnM]
                   + PdbVolume[SectionM][Row][ColumnP]
                   - PdbVolume[Section][RowM][ColumnM]
                   + PdbVolume[Section][RowM][ColumnP]
                   - PdbVolume[Section][Row][ColumnM]
		   + PdbVolume[Section][Row][ColumnP];
	      }}} else {
                if(Column == 0) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   - PdbVolume[SectionM][Row][0]
                   - PdbVolume[SectionM][Row][1]
                   - PdbVolume[SectionM][RowP][0]
                   - PdbVolume[SectionM][RowP][1];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   + PdbVolume[SectionM][RowP][0]
                   + PdbVolume[SectionM][RowP][1]
                   - PdbVolume[Section][RowM][0]
                   - PdbVolume[Section][RowM][1]
                   + PdbVolume[Section][RowP][0]
                   + PdbVolume[Section][RowP][1];
                  PdbVector[ExtremaCounter - 1] =
                     PdbVolume[SectionM][RowM][1]
                   + PdbVolume[SectionM][Row][1]
                   + PdbVolume[SectionM][RowP][1]
                   + PdbVolume[Section][RowM][1]
                   + PdbVolume[Section][Row][1]
                   + PdbVolume[Section][RowP][1];
	        } else {	
                  if(Column == PdbColumnD - 1) {
                    PdbVector[ExtremaCounter - 3] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][Row][Column]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     - PdbVolume[SectionM][RowP][Column];
                    PdbVector[ExtremaCounter - 2] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     + PdbVolume[SectionM][RowP][ColumnM]
                     + PdbVolume[SectionM][RowP][Column]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][RowM][Column]
                     + PdbVolume[Section][RowP][ColumnM]
                     + PdbVolume[Section][RowP][Column];
                    PdbVector[ExtremaCounter - 1] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][Row][ColumnM]
                     - PdbVolume[Section][RowP][ColumnM];
		  } else {
                    PdbVector[ExtremaCounter - 3] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][RowM][ColumnP]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][Row][Column]
                     - PdbVolume[SectionM][Row][ColumnP]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     - PdbVolume[SectionM][RowP][Column]
                     - PdbVolume[SectionM][RowP][ColumnP];
                    PdbVector[ExtremaCounter - 2] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][RowM][ColumnP]
                     + PdbVolume[SectionM][RowP][ColumnM]
                     + PdbVolume[SectionM][RowP][Column]
                     + PdbVolume[SectionM][RowP][ColumnP]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][RowM][Column]
                     - PdbVolume[Section][RowM][ColumnP]
                     + PdbVolume[Section][RowP][ColumnM]
                     + PdbVolume[Section][RowP][Column]
 	             + PdbVolume[Section][RowP][ColumnP];
                    PdbVector[ExtremaCounter - 1] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     + PdbVolume[SectionM][RowM][ColumnP]
                     - PdbVolume[SectionM][Row][ColumnM]
                     + PdbVolume[SectionM][Row][ColumnP]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     + PdbVolume[SectionM][RowP][ColumnP]
                     - PdbVolume[Section][RowM][ColumnM]
                     + PdbVolume[Section][RowM][ColumnP]
                     - PdbVolume[Section][Row][ColumnM]
                     + PdbVolume[Section][Row][ColumnP]
                     - PdbVolume[Section][RowP][ColumnM]
                     + PdbVolume[Section][RowP][ColumnP];
	  }}}}} else {
            if(Row == 0) {
              if(Column == 0) {
                PdbVector[ExtremaCounter - 3] =
                  -PdbVolume[SectionM][0][0]
                 - PdbVolume[SectionM][0][1]
                 - PdbVolume[SectionM][1][0]
                 - PdbVolume[SectionM][1][1]
                 + PdbVolume[SectionP][0][0]
                 + PdbVolume[SectionP][0][1]
                 + PdbVolume[SectionP][1][0]
                 + PdbVolume[SectionP][1][1];
                PdbVector[ExtremaCounter - 2] =
                   PdbVolume[SectionM][1][0]
                 + PdbVolume[SectionM][1][1]
                 + PdbVolume[Section][1][0]
                 + PdbVolume[Section][1][1]
                 + PdbVolume[SectionP][1][0]
                 + PdbVolume[SectionP][1][1];
                PdbVector[ExtremaCounter - 1] =
                   PdbVolume[SectionM][0][1]
                 + PdbVolume[SectionM][1][1]
                 + PdbVolume[Section][0][1]
                 + PdbVolume[Section][1][1]
                 + PdbVolume[SectionP][0][1]
                 + PdbVolume[SectionP][1][1];
	      } else {
                if(Column == PdbColumnD - 1) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][0][ColumnM]
                   - PdbVolume[SectionM][0][Column]
                   - PdbVolume[SectionM][1][ColumnM]
                   - PdbVolume[SectionM][1][Column]
                   + PdbVolume[SectionP][0][ColumnM]
                   + PdbVolume[SectionP][0][Column]
                   + PdbVolume[SectionP][1][ColumnM]
                   + PdbVolume[SectionP][1][Column];
                  PdbVector[ExtremaCounter - 2] =
                     PdbVolume[SectionM][1][ColumnM]
                   + PdbVolume[SectionM][1][Column]
                   + PdbVolume[Section][1][ColumnM]
                   + PdbVolume[Section][1][Column]
                   + PdbVolume[SectionP][1][ColumnM]
                   + PdbVolume[SectionP][1][Column];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][0][ColumnM]
                   - PdbVolume[SectionM][1][ColumnM]
                   - PdbVolume[Section][0][ColumnM]
                   - PdbVolume[Section][1][ColumnM]
                   - PdbVolume[SectionP][0][ColumnM]
                   - PdbVolume[SectionP][1][ColumnM];
                } else {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][0][ColumnM]
                   - PdbVolume[SectionM][0][Column]
                   - PdbVolume[SectionM][0][ColumnP]
                   - PdbVolume[SectionM][1][ColumnM]
                   - PdbVolume[SectionM][1][Column]
                   - PdbVolume[SectionM][1][ColumnP]
                   + PdbVolume[SectionP][0][ColumnM]
                   + PdbVolume[SectionP][0][Column]
                   + PdbVolume[SectionP][0][ColumnP]
                   + PdbVolume[SectionP][1][ColumnM]
                   + PdbVolume[SectionP][1][Column]
                   + PdbVolume[SectionP][1][ColumnP];
                  PdbVector[ExtremaCounter - 2] =
                     PdbVolume[SectionM][1][ColumnM]
                   + PdbVolume[SectionM][1][Column]
                   + PdbVolume[SectionM][1][ColumnP]
                   + PdbVolume[Section][1][ColumnM]
                   + PdbVolume[Section][1][Column]
                   + PdbVolume[Section][1][ColumnP]
                   + PdbVolume[SectionP][1][ColumnM]
                   + PdbVolume[SectionP][1][Column]
                   + PdbVolume[SectionP][1][ColumnP];
                  PdbVector[ExtremaCounter - 1] =
                    -PdbVolume[SectionM][0][ColumnM]
                   + PdbVolume[SectionM][0][ColumnP]
                   - PdbVolume[SectionM][1][ColumnM]
                   + PdbVolume[SectionM][1][ColumnP]
                   - PdbVolume[Section][0][ColumnM]
                   + PdbVolume[Section][0][ColumnP]
                   - PdbVolume[Section][1][ColumnM]
                   + PdbVolume[Section][1][ColumnP]
                   - PdbVolume[SectionP][0][ColumnM]
                   + PdbVolume[SectionP][0][ColumnP]
                   - PdbVolume[SectionP][1][ColumnM]
                 + PdbVolume[SectionP][1][ColumnP];
            }}} else {
	      if(Row == PdbRowD - 1) {
                if(Column == 0) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   - PdbVolume[SectionM][Row][0]
                   - PdbVolume[SectionM][Row][1]
                   + PdbVolume[SectionP][RowM][0]
                   + PdbVolume[SectionP][RowM][1]
                   + PdbVolume[SectionP][Row][0]
                   + PdbVolume[SectionP][Row][1];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   - PdbVolume[Section][RowM][0]
                   - PdbVolume[Section][RowM][1]
                   - PdbVolume[SectionP][RowM][0]
                   - PdbVolume[SectionP][RowM][1];
                  PdbVector[ExtremaCounter - 1] =
                     PdbVolume[SectionM][RowM][1]
                   + PdbVolume[SectionM][Row][1]
                   + PdbVolume[Section][RowM][1]
                   + PdbVolume[Section][Row][1]
                   + PdbVolume[SectionP][RowM][1]
                   + PdbVolume[SectionP][Row][1];
                } else {
                  if(Column == PdbColumnD - 1) {
                    PdbVector[ExtremaCounter - 3] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][Row][Column]
                     + PdbVolume[SectionP][RowM][ColumnM]
                     + PdbVolume[SectionP][RowM][Column]
                     + PdbVolume[SectionP][Row][ColumnM]
                     + PdbVolume[SectionP][Row][Column];
                    PdbVector[ExtremaCounter - 2] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][RowM][Column]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     - PdbVolume[SectionP][RowM][Column];
                    PdbVector[ExtremaCounter - 1] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][Row][ColumnM]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     - PdbVolume[SectionP][Row][ColumnM];
                  } else {
                    PdbVector[ExtremaCounter - 3] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][RowM][ColumnP]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][Row][Column]
                     - PdbVolume[SectionM][Row][ColumnP]
                     + PdbVolume[SectionP][RowM][ColumnM]
                     + PdbVolume[SectionP][RowM][Column]
                     + PdbVolume[SectionP][RowM][ColumnP]
                     + PdbVolume[SectionP][Row][ColumnM]
                     + PdbVolume[SectionP][Row][Column]
                     + PdbVolume[SectionP][Row][ColumnP];
                    PdbVector[ExtremaCounter - 2] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][RowM][ColumnP]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][RowM][Column]
                     - PdbVolume[Section][RowM][ColumnP]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     - PdbVolume[SectionP][RowM][Column]
                      - PdbVolume[SectionP][RowM][ColumnP];
                    PdbVector[ExtremaCounter - 1] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     + PdbVolume[SectionM][RowM][ColumnP]
                     - PdbVolume[SectionM][Row][ColumnM]
                     + PdbVolume[SectionM][Row][ColumnP]
                     - PdbVolume[Section][RowM][ColumnM]
                     + PdbVolume[Section][RowM][ColumnP]
                     - PdbVolume[Section][Row][ColumnM]
                     + PdbVolume[Section][Row][ColumnP]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     + PdbVolume[SectionP][RowM][ColumnP]
                     - PdbVolume[SectionP][Row][ColumnM]
                     + PdbVolume[SectionP][Row][ColumnP];
              }}} else {
                if(Column == 0) {
                  PdbVector[ExtremaCounter - 3] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   - PdbVolume[SectionM][Row][0]
                   - PdbVolume[SectionM][Row][1]
                   - PdbVolume[SectionM][RowP][0]
                   - PdbVolume[SectionM][RowP][1]
                   + PdbVolume[SectionP][RowM][0]
                   + PdbVolume[SectionP][RowM][1]
                   + PdbVolume[SectionP][Row][0]
                   + PdbVolume[SectionP][Row][1]
                   + PdbVolume[SectionP][RowP][0]
                   + PdbVolume[SectionP][RowP][1];
                  PdbVector[ExtremaCounter - 2] =
                    -PdbVolume[SectionM][RowM][0]
                   - PdbVolume[SectionM][RowM][1]
                   + PdbVolume[SectionM][RowP][0]
                   + PdbVolume[SectionM][RowP][1]
                   - PdbVolume[Section][RowM][0]
                   - PdbVolume[Section][RowM][1]
                   + PdbVolume[Section][RowP][0]
                   + PdbVolume[Section][RowP][1]
                   - PdbVolume[SectionP][RowM][0]
                   - PdbVolume[SectionP][RowM][1]
                   + PdbVolume[SectionP][RowP][0]
                   + PdbVolume[SectionP][RowP][1];
                  PdbVector[ExtremaCounter - 1] =
                     PdbVolume[SectionM][RowM][1]
                   + PdbVolume[SectionM][Row][1]
                   + PdbVolume[SectionM][RowP][1]
                   + PdbVolume[Section][RowM][1]
                   + PdbVolume[Section][Row][1]
                   + PdbVolume[Section][RowP][1]
                   + PdbVolume[SectionP][RowM][1]
                   + PdbVolume[SectionP][Row][1]
                   + PdbVolume[SectionP][RowP][1];
                } else {
	          if(Column == PdbColumnD - 1) {
                    PdbVector[ExtremaCounter - 3] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][Row][Column]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     - PdbVolume[SectionM][RowP][Column]
                     + PdbVolume[SectionP][RowM][ColumnM]
                     + PdbVolume[SectionP][RowM][Column]
                     + PdbVolume[SectionP][Row][ColumnM]
                     + PdbVolume[SectionP][Row][Column]
                     + PdbVolume[SectionP][RowP][ColumnM]
                     + PdbVolume[SectionP][RowP][Column];
                    PdbVector[ExtremaCounter - 2] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     + PdbVolume[SectionM][RowP][ColumnM]
                     + PdbVolume[SectionM][RowP][Column]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][RowM][Column]
                     + PdbVolume[Section][RowP][ColumnM]
                     + PdbVolume[Section][RowP][Column]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     - PdbVolume[SectionP][RowM][Column]
                     + PdbVolume[SectionP][RowP][ColumnM]
                     + PdbVolume[SectionP][RowP][Column];
                    PdbVector[ExtremaCounter - 1] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][Row][ColumnM]
                     - PdbVolume[Section][RowP][ColumnM]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     - PdbVolume[SectionP][Row][ColumnM]
                     - PdbVolume[SectionP][RowP][ColumnM];
                  } else {
                    PdbVector[ExtremaCounter - 3] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][RowM][ColumnP]
                     - PdbVolume[SectionM][Row][ColumnM]
                     - PdbVolume[SectionM][Row][Column]
                     - PdbVolume[SectionM][Row][ColumnP]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     - PdbVolume[SectionM][RowP][Column]
                     - PdbVolume[SectionM][RowP][ColumnP]
                     + PdbVolume[SectionP][RowM][ColumnM]
                     + PdbVolume[SectionP][RowM][Column]
                     + PdbVolume[SectionP][RowM][ColumnP]
                     + PdbVolume[SectionP][Row][ColumnM]
                     + PdbVolume[SectionP][Row][Column]
                     + PdbVolume[SectionP][Row][ColumnP]
                     + PdbVolume[SectionP][RowP][ColumnM]
                     + PdbVolume[SectionP][RowP][Column]
                     + PdbVolume[SectionP][RowP][ColumnP];
                    PdbVector[ExtremaCounter - 2] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     - PdbVolume[SectionM][RowM][Column]
                     - PdbVolume[SectionM][RowM][ColumnP]
                     + PdbVolume[SectionM][RowP][ColumnM]
                     + PdbVolume[SectionM][RowP][Column]
                     + PdbVolume[SectionM][RowP][ColumnP]
                     - PdbVolume[Section][RowM][ColumnM]
                     - PdbVolume[Section][RowM][Column]
                     - PdbVolume[Section][RowM][ColumnP]
                     + PdbVolume[Section][RowP][ColumnM]
                     + PdbVolume[Section][RowP][Column]
                     + PdbVolume[Section][RowP][ColumnP]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     - PdbVolume[SectionP][RowM][Column]
                     - PdbVolume[SectionP][RowM][ColumnP]
                     + PdbVolume[SectionP][RowP][ColumnM]
                     + PdbVolume[SectionP][RowP][Column]
                     + PdbVolume[SectionP][RowP][ColumnP];
                    PdbVector[ExtremaCounter - 1] =
                      -PdbVolume[SectionM][RowM][ColumnM]
                     + PdbVolume[SectionM][RowM][ColumnP]
                     - PdbVolume[SectionM][Row][ColumnM]
                     + PdbVolume[SectionM][Row][ColumnP]
                     - PdbVolume[SectionM][RowP][ColumnM]
                     + PdbVolume[SectionM][RowP][ColumnP]
                     - PdbVolume[Section][RowM][ColumnM]
                     + PdbVolume[Section][RowM][ColumnP]
                     - PdbVolume[Section][Row][ColumnM]
                     + PdbVolume[Section][Row][ColumnP]
                     - PdbVolume[Section][RowP][ColumnM]
                     + PdbVolume[Section][RowP][ColumnP]
                     - PdbVolume[SectionP][RowM][ColumnM]
                     + PdbVolume[SectionP][RowM][ColumnP]
                     - PdbVolume[SectionP][Row][ColumnM]
                     + PdbVolume[SectionP][Row][ColumnP]
                     - PdbVolume[SectionP][RowP][ColumnM]
                     + PdbVolume[SectionP][RowP][ColumnP];
  }}}}}}}}} printf("\nNumber of test set voxels : %d\n\n", ExtremaCounter / 3);

  /* free the memory allocated to the surface and volume array of the model */
  free(PdbSurfaceValues);
  free(PdbVolumeValues);
  for(Section = 0; Section < PdbSectionD; Section++) {
    free(PdbSurface[Section]);
    free(PdbVolume[Section]);
  } free(PdbSurface);
  free(PdbVolume);

  /* precalculation of inner rotation cos(angle) and sin(angle) values */
  RotationSteps *= 2;
  TrigValues = malloc(RotationSteps * sizeof(float));
  OutOfMemory(TrigValues);
  for(Index1 = 0; Index1 < RotationSteps; Index1 += 2) {
    TrigValues[Index1] = cosf(Index1 * TwoPi / RotationSteps);
    TrigValues[Index1 + 1] = sinf(Index1 * TwoPi / RotationSteps);
  }

  /* initialise the score classes array */
  /* int by integer casting */
  TresholdScore = MinSurfaceOverlap * ExtremaCounter;
  SolutionCounter = 0;
  NumberOfScoreClasses = (ExtremaCounter + 2 - TresholdScore) / 3;
  ScoreClasses = calloc(NumberOfScoreClasses, sizeof(float *));
  OutOfMemory(ScoreClasses);
  ScoreClassCounters = calloc(NumberOfScoreClasses, sizeof(long int));
  OutOfMemory(ScoreClassCounters);
  --NumberOfScoreClasses;
   
  /* the actual search : the extrema set voxels and the associated vector are */
  /* superimposed onto all the surface voxels and the associated vector, */
  /* followed by a rotational search round the superimposed vectors */
  for(Index1 = 0; Index1 < ExtremaCounter;) {
    Section = Extrema[Index1]; /* model voxel Z coordinate */
    z = PdbVector[Index1++];
    Row = Extrema[Index1]; /* model voxel Y coordinate */
    y = PdbVector[Index1++];
    Column = Extrema[Index1]; /* model voxel X coordinate */
    x = PdbVector[Index1++];
    Length = sqrtf(z * z + y * y + x * x); /* length model voxel vector */
    printf("Test voxel # %d\n", Index1 / 3);

    /* if the vector is not the zero vector, normalise it and superimpose it */
    if(Length) {
      Z = z / Length; /* model voxel vector Z coordinate */
      Y = y / Length; /* model voxel vector Y coordinate */
      X = x / Length; /* model voxel vector X coordinate */
      for(Index2 = 0; Index2 < CoordCounter;) {
	SectionM = Coord[Index2]; /* map voxel Z coordinate */
        Z2 = Vector[Index2++]; /* map voxel vector Z coordinate */
	RowM = Coord[Index2]; /* map voxel Y coordinate */
        Y2 = Vector[Index2++]; /* map voxel vector Y coordinate */
	ColumnM = Coord[Index2]; /* map voxel X coordinate */
        X2 = Vector[Index2++]; /* map voxel vector X coordinate */
        Z3 = X * Y2 - Y * X2; /* rotation axis vector Z coordinate */
	Y3 = Z * X2 - X * Z2; /* rotation axis vector Y coordinate */
	X3 = Y * Z2 - Z * Y2; /* rotation axis vector X coordinate */
	Length = sqrtf(Z3 * Z3 + Y3 * Y3 + X3 * X3); /* length axis vector */

	/* if the model and map vectors are not identical, calculate */
	/* the required rotation matrices and translation vector */
        /* -> outer rotation matrix */
	if(Length) {
	  Z3 /= Length; /* normalisation of the axis vector */
	  Y3 /= Length;
	  X3 /= Length;
	  CosAngle = Z * Z2 + Y * Y2 + X * X2;
	  SinAngle = sqrtf(1 - CosAngle * CosAngle);
	  OneMinCosAngle = 1 - CosAngle;
	  XSinAngle = X3 * SinAngle;
	  YSinAngle = Y3 * SinAngle;
	  ZSinAngle = Z3 * SinAngle;
	  Ra11 = X3 * OneMinCosAngle;
	  Ra22 = Y3 * OneMinCosAngle;
	  Ra33 = Z3 * OneMinCosAngle;
	  Ra12 = X3 * Ra22;
	  Ra21 = Ra12 + ZSinAngle;
	  Ra12 -= ZSinAngle;
	  Ra13 = Z3 * Ra11;
	  Ra31 = Ra13 - YSinAngle;
	  Ra13 += YSinAngle;
	  Ra23 = Y3 * Ra33;
	  Ra32 = Ra23 + XSinAngle;
	  Ra23 -= XSinAngle;	
	  Ra11 = X3 * Ra11 + CosAngle;
	  Ra22 = Y3 * Ra22 + CosAngle;
	  Ra33 = Z3 * Ra33 + CosAngle;
	} else {
          Ra11 = 1; /* identity matrix */
          Ra12 = 0;
          Ra13 = 0;
          Ra21 = 0;
          Ra22 = 1;
          Ra23 = 0;
          Ra31 = 0;
          Ra32 = 0;
          Ra33 = 1;
        }

        /* -> inner rotation matrix */
	for(Index3 = 0; Index3 < RotationSteps; Index3 += 2) {
	  CosAngle = *(TrigValues + Index3);
	  SinAngle = *(TrigValues + Index3 + 1);
	  OneMinCosAngle = 1 - CosAngle;
	  XSinAngle = X2 * SinAngle;
	  YSinAngle = Y2 * SinAngle;
	  ZSinAngle = Z2 * SinAngle;
 	  Rb11 = X2 * OneMinCosAngle;
	  Rb22 = Y2 * OneMinCosAngle;
	  Rb33 = Z2 * OneMinCosAngle;
	  Rb12 = X2 * Rb22;
	  Rb21 = Rb12 + ZSinAngle;
	  Rb12 -= ZSinAngle;
	  Rb13 = Z2 * Rb11;
	  Rb31 = Rb13 - YSinAngle;
	  Rb13 += YSinAngle;
	  Rb23 = Y2 * Rb33;
	  Rb32 = Rb23 + XSinAngle;
	  Rb23 -= XSinAngle;	
	  Rb11 = X2 * Rb11 + CosAngle;
	  Rb22 = Y2 * Rb22 + CosAngle;
	  Rb33 = Z2 * Rb33 + CosAngle;

          /* -> combined rotation matrix */
	  Rc11 = Rb11 * Ra11 + Rb12 * Ra21 + Rb13 * Ra31;
	  Rc12 = Rb11 * Ra12 + Rb12 * Ra22 + Rb13 * Ra32;
	  Rc13 = Rb11 * Ra13 + Rb12 * Ra23 + Rb13 * Ra33;
	  Rc21 = Rb21 * Ra11 + Rb22 * Ra21 + Rb23 * Ra31;
	  Rc22 = Rb21 * Ra12 + Rb22 * Ra22 + Rb23 * Ra32;
	  Rc23 = Rb21 * Ra13 + Rb22 * Ra23 + Rb23 * Ra33;
	  Rc31 = Rb31 * Ra11 + Rb32 * Ra21 + Rb33 * Ra31;
	  Rc32 = Rb31 * Ra12 + Rb32 * Ra22 + Rb33 * Ra32;
	  Rc33 = Rb31 * Ra13 + Rb32 * Ra23 + Rb33 * Ra33;

	  /* -> combined translation vector */
	  TcZ = 0.5 + SectionM - Rc33 * Section - Rc32 * Row - Rc31 * Column;
          TcY = 0.5 + RowM - Rc23 * Section - Rc22 * Row - Rc21 * Column;
          TcX = 0.5 + ColumnM - Rc13 * Section - Rc12 * Row - Rc11 * Column;

          /* scoring */
          Score = 0;
          AllowedZeroSteps = ExtremaCounter - TresholdScore;
          for(Index4 = 0; Index4 < ExtremaCounter;) {
            SectionP = Extrema[Index4++];
            RowP = Extrema[Index4++];
            ColumnP = Extrema[Index4++];
            /* int by integer casting */
            z = Rc33 * SectionP + Rc32 * RowP + Rc31 * ColumnP + TcZ;
            y = Rc23 * SectionP + Rc22 * RowP + Rc21 * ColumnP + TcY;
            x = Rc13 * SectionP + Rc12 * RowP + Rc11 * ColumnP + TcX;
            if(z < 0 || y < 0 || x < 0 || z >= SectionD || y >= RowD || x >= ColumnD) {
	      Score = 0;
	      break;
	    } if(Scoring[z][y][x]) {
	      Score += 3;
	    } else {
	      AllowedZeroSteps -= 3;
	      if(AllowedZeroSteps < 0) break;   
	  }}

          /* if the score is high enough, save the parameters */
          if(Score > TresholdScore) {
            Score -= ExtremaCounter;
            Score /= -3;
            SolutionCounter += 12;
            ScoreClassCounters[Score] += 12;
            ScoreClasses[Score] = realloc(
             ScoreClasses[Score], ScoreClassCounters[Score] * sizeof(float));
            OutOfMemory(ScoreClasses[Score]);
            ParameterPointer = ScoreClasses[Score] + ScoreClassCounters[Score];
            *(ParameterPointer - 12) = Rc11;
            *(ParameterPointer - 11) = Rc12;
            *(ParameterPointer - 10) = Rc13;
            *(ParameterPointer - 9) = Rc21;
            *(ParameterPointer - 8) = Rc22;
            *(ParameterPointer - 7) = Rc23;
            *(ParameterPointer - 6) = Rc31;
            *(ParameterPointer - 5) = Rc32;
            *(ParameterPointer - 4) = Rc33;
            *(ParameterPointer - 3) = TcX;
            *(ParameterPointer - 2) = TcY;
            *(ParameterPointer - 1) = TcZ;
        }}

        /* reduce the number of saved solutions to a none zero number */
        /* as close as possible to 2000 */
        while(
              (SolutionCounter > 24000 +
               ScoreClassCounters[NumberOfScoreClasses] / 2)&&
              (SolutionCounter > ScoreClassCounters[NumberOfScoreClasses])
             ) {
          SolutionCounter -= ScoreClassCounters[NumberOfScoreClasses];
          free(ScoreClasses[NumberOfScoreClasses]);
          --NumberOfScoreClasses;
          TresholdScore += 3;
  }}}} ++NumberOfScoreClasses;
  realloc(TrigValues, 2 * sizeof(float));
  printf("\nRe-scoring the retained solutions ...\n\n");
  
  /* for all remaining solutions (and for 6 variants resulting from an */
  /*  additional orthogonal translation over 1 voxel), calculate the full score */
  Solutions = realloc(Solutions, (SolutionCounter / 12) * 16 * sizeof(float));
  SolutionCounter = 0;
  for(Index1 = 0; Index1 < NumberOfScoreClasses; Index1++) {
    for(Index2 = 0; Index2 < ScoreClassCounters[Index1]; Index2 += 12) {
      ParameterPointer = ScoreClasses[Index1] + Index2;
      Rc11 = *(ParameterPointer);
      Rc12 = *(ParameterPointer + 1);
      Rc13 = *(ParameterPointer + 2);
      Rc21 = *(ParameterPointer + 3);
      Rc22 = *(ParameterPointer + 4);
      Rc23 = *(ParameterPointer + 5);
      Rc31 = *(ParameterPointer + 6);
      Rc32 = *(ParameterPointer + 7);
      Rc33 = *(ParameterPointer + 8);
      TcX = *(ParameterPointer + 9);
      TcY = *(ParameterPointer + 10);
      TcZ = *(ParameterPointer + 11);

      /* scoring */
      Index4 = 0;
      ScoreU = 0;
      ScoreD = 0;
      ScoreN = 0;
      ScoreE = 0;
      ScoreS = 0;
      ScoreW = 0;
      for(Index3 = 0; Index3 < PdbCoordCounter;) {
        Section = PdbCoord[Index3++];
        Row = PdbCoord[Index3++];
        Column = PdbCoord[Index3++];
        /* int by integer casting */
        z = Rc33 * Section + Rc32 * Row + Rc31 * Column + TcZ;
        y = Rc23 * Section + Rc22 * Row + Rc21 * Column + TcY;
        x = Rc13 * Section + Rc12 * Row + Rc11 * Column + TcX;
	SectionM = z - 1;
	RowM = y - 1;
	ColumnM = x - 1;
	SectionP = z + 1;
	RowP = y + 1;
	ColumnP = x + 1;
        if(z < 0 || y < 0 || x < 0 || z >= SectionD || y >= RowD || x >= ColumnD) {
          Index4 = -PdbCoordCounter;
	} else {
          if(Scoring[z][y][x]) Index4 += 3;
        } if(SectionM < 0 || y < 0 || x < 0 || SectionM >= SectionD || y >= RowD || x >= ColumnD) {
          ScoreD = -PdbCoordCounter;
	} else {
          if(Scoring[SectionM][y][x]) ScoreD += 3;
	} if(SectionP < 0 || y < 0 || x < 0 || SectionP >= SectionD || y >= RowD || x >= ColumnD) {
          ScoreU = -PdbCoordCounter;
	} else {
          if(Scoring[SectionP][y][x]) ScoreU += 3;
	} if(z < 0 || RowM < 0 || x < 0 || z >= SectionD || RowM >= RowD || x >= ColumnD) {
          ScoreS = -PdbCoordCounter;
	} else {
          if(Scoring[z][RowM][x]) ScoreS += 3;
        } if(z < 0 || RowP < 0 || x < 0 || z >= SectionD || RowP >= RowD || x >= ColumnD) {
          ScoreN = -PdbCoordCounter;
	} else {
          if(Scoring[z][RowP][x]) ScoreN += 3;
	} if(z < 0 || y < 0 || ColumnM < 0 || z >= SectionD || y >= RowD || ColumnM >= ColumnD) {
          ScoreW = -PdbCoordCounter;
	} else {
          if(Scoring[z][y][ColumnM]) ScoreW += 3;
	} if(z < 0 || y < 0 || ColumnP < 0 || z >= SectionD || y >= RowD || ColumnP >= ColumnD) {
          ScoreE = -PdbCoordCounter;
	} else {
          if(Scoring[z][y][ColumnP]) ScoreE += 3;
      }}

      /* out of these seven transformations, the one yielding the highest score is kept */
      Score = Index4;
      if(ScoreU > Score) Score = ScoreU;
      if(ScoreD > Score) Score = ScoreD;
      if(ScoreN > Score) Score = ScoreN;
      if(ScoreE > Score) Score = ScoreE;
      if(ScoreS > Score) Score = ScoreS;
      if(ScoreW > Score) Score = ScoreW;
      if(Score != Index4) {
        if(Score == ScoreU) {
	  ++TcZ;
	} else {
	  if(Score == ScoreD) {
	    --TcZ;
	  } else {
	    if(Score == ScoreN) {
	      ++TcY;
	    } else {
	      if(Score == ScoreE) {
		++TcX;
	      } else {
		if(Score == ScoreS) {
		  --TcY;
		} else {
		  --TcX;
      }}}}}} while(Index4 < Score) {
	Index4 = Score;
        ScoreU = 0;
        ScoreD = 0;
        ScoreN = 0;
        ScoreE = 0;
        ScoreS = 0;
        ScoreW = 0;
        for(Index3 = 0; Index3 < PdbCoordCounter;) {
          Section = PdbCoord[Index3++];
          Row = PdbCoord[Index3++];
          Column = PdbCoord[Index3++];
          /* int by integer casting */
          z = Rc33 * Section + Rc32 * Row + Rc31 * Column + TcZ;
          y = Rc23 * Section + Rc22 * Row + Rc21 * Column + TcY;
          x = Rc13 * Section + Rc12 * Row + Rc11 * Column + TcX;
	  SectionM = z - 1;
	  RowM = y - 1;
	  ColumnM = x - 1;
	  SectionP = z + 1;
	  RowP = y + 1;
	  ColumnP = x + 1;
          if(SectionM < 0 || y < 0 || x < 0 || SectionM >= SectionD || y >= RowD || x >= ColumnD) {
            ScoreD = -PdbCoordCounter;
	  } else {
            if(Scoring[SectionM][y][x]) ScoreD += 3;
	  } if(SectionP < 0 || y < 0 || x < 0 || SectionP >= SectionD || y >= RowD || x >= ColumnD) {
            ScoreU = -PdbCoordCounter;
	  } else {
            if(Scoring[SectionP][y][x]) ScoreU += 3;
	  } if(z < 0 || RowM < 0 || x < 0 || z >= SectionD || RowM >= RowD || x >= ColumnD) {
            ScoreS = -PdbCoordCounter;
	  } else {
            if(Scoring[z][RowM][x]) ScoreS += 3;
          } if(z < 0 || RowP < 0 || x < 0 || z >= SectionD || RowP >= RowD || x >= ColumnD) {
            ScoreN = -PdbCoordCounter;
	  } else {
            if(Scoring[z][RowP][x]) ScoreN += 3;
	  } if(z < 0 || y < 0 || ColumnM < 0 || z >= SectionD || y >= RowD || ColumnM >= ColumnD) {
            ScoreW = -PdbCoordCounter;
	  } else {
            if(Scoring[z][y][ColumnM]) ScoreW += 3;
	  } if(z < 0 || y < 0 || ColumnP < 0 || z >= SectionD || y >= RowD || ColumnP >= ColumnD) {
            ScoreE = -PdbCoordCounter;
	  } else {
            if(Scoring[z][y][ColumnP]) ScoreE += 3;
        }} if(ScoreU > Score) Score = ScoreU;
        if(ScoreD > Score) Score = ScoreD;
        if(ScoreN > Score) Score = ScoreN;
        if(ScoreE > Score) Score = ScoreE;
        if(ScoreS > Score) Score = ScoreS;
        if(ScoreW > Score) Score = ScoreW;
        if(Score != Index4) {
        if(Score == ScoreU) {
	  ++TcZ;
	} else {
	  if(Score == ScoreD) {
	    --TcZ;
	  } else {
	    if(Score == ScoreN) {
	      ++TcY;
	    } else {
	      if(Score == ScoreE) {
		++TcX;
	      } else {
		if(Score == ScoreS) {
		  --TcY;
		} else {
		  --TcX;
      }}}}}}}

      /* if the full score is high enough, calculate the cross-correlation  */
      if(Score > TresholdScore) {
        CrossCorrelation(PdbDensityCoord,
                         PdbFinalValues,
                         PdbVolumeVoxelCounter,
                         Density,
                         SectionD - 2,
                         RowD - 2,
                         ColumnD - 2,
                         ParameterPointer,
			 TcX,
			 TcY,
			 TcZ,
			 TrigValues);
	/* if the cross-correlation is > -2, ... */
	if(*TrigValues > -2.0) {
	  /* calculate the ranking of the combined score (binary search) ... */
	  Length = Score / (2.0 * PdbCoordCounter) + (TrigValues[0] + TrigValues[1]) / 4.0 ;
	  x = -16;
	  X = -2;
	  y = SolutionCounter;
	  Y = 2;
	  while(y - x > 16) {
	    z = (x + y) / 32;
	    z *= 16;
	    Z = Solutions[z];
	    if(Length <= Z) {
	      y = z;
	      Y = Z;
	    } else {
	      x = z;
	      X = Z;
	  }}

	  /* and save the parameters */
          ParameterPointer = Solutions + y;
	  memmove(ParameterPointer + 16, ParameterPointer, 4 * (SolutionCounter - y));
          *(ParameterPointer) = Length;
	  *(ParameterPointer + 1) = Score;
	  *(ParameterPointer + 1) /= PdbCoordCounter;
	  *(ParameterPointer + 2) = TrigValues[0];
          *(ParameterPointer + 3) = TrigValues[1];
          *(ParameterPointer + 4) = Rc11;
          *(ParameterPointer + 5) = Rc12;
          *(ParameterPointer + 6) = Rc13;
          *(ParameterPointer + 7) = Rc21;
          *(ParameterPointer + 8) = Rc22;
          *(ParameterPointer + 9) = Rc23;
          *(ParameterPointer + 10) = Rc31;
          *(ParameterPointer + 11) = Rc32;
          *(ParameterPointer + 12) = Rc33;
          *(ParameterPointer + 13) = TcX - 0.5;
          *(ParameterPointer + 14) = TcY - 0.5;
          *(ParameterPointer + 15) = TcZ - 0.5;
	  SolutionCounter += 16;
    }}}

    /* free the memory allocated to the initial score arrays */ 
    free(ScoreClasses[Index1]);
  } free(ScoreClasses);
  free(ScoreClassCounters);
  Solutions = realloc(Solutions, SolutionCounter * sizeof(float));

  /* remove redundant (MSD < VoxelSize) solutions that would be reported */
  /* from the list */
  if(SolutionCounter < ToConvertNumber * 16) ToConvertNumber = SolutionCounter;
  for(Index1 = 1; Index1 < ToConvertNumber; Index1++) {
    ParameterPointer = Solutions + (SolutionCounter - Index1 * 16);
    Rc11 = *(ParameterPointer - 12);
    Rc12 = *(ParameterPointer - 11);
    Rc13 = *(ParameterPointer - 10);
    Rc21 = *(ParameterPointer - 9);
    Rc22 = *(ParameterPointer - 8);
    Rc23 = *(ParameterPointer - 7);
    Rc31 = *(ParameterPointer - 6);
    Rc32 = *(ParameterPointer - 5);
    Rc33 = *(ParameterPointer - 4);
    TcX = *(ParameterPointer - 3);
    TcY = *(ParameterPointer - 2);
    TcZ = *(ParameterPointer - 1);
    for(Index2 = 0; Index2 < Index1; Index2 ++) {
      ParameterPointer = Solutions + (SolutionCounter - Index2 * 16);
      Ra11 = Rc11 - *(ParameterPointer - 12);
      Ra12 = Rc12 - *(ParameterPointer - 11);
      Ra13 = Rc13 - *(ParameterPointer - 10);
      Ra21 = Rc21 - *(ParameterPointer - 9);
      Ra22 = Rc22 - *(ParameterPointer - 8);
      Ra23 = Rc23 - *(ParameterPointer - 7);
      Ra31 = Rc31 - *(ParameterPointer - 6);
      Ra32 = Rc32 - *(ParameterPointer - 5);
      Ra33 = Rc33 - *(ParameterPointer - 4);
      X = TcX - *(ParameterPointer - 3);
      Y = TcY - *(ParameterPointer - 2);
      Z = TcZ - *(ParameterPointer - 1);    
      /* calculation of the MSD */
      Value = 0;
      for(Index3 = 0; Index3 < ExtremaCounter; Index3 += 3) {
        Section = Extrema[Index3];
        Row = Extrema[Index3 + 1];
        Column = Extrema[Index3 + 2];
        Z2 = Ra33 * Section + Ra32 * Row + Ra31 * Column + Z;
        Y2 = Ra23 * Section + Ra22 * Row + Ra21 * Column + Y;
        X2 = Ra13 * Section + Ra12 * Row + Ra11 * Column + X;
        Value += Z2 * Z2 + Y2 * Y2 + X2 * X2;
      } Value /= ExtremaCounter / 3;
      if(Value <= 3.0) {
        memmove(Solutions + (SolutionCounter - (Index1 + 1) * 16), Solutions + (SolutionCounter - Index1 * 16), 64 * Index1);
        SolutionCounter -= 16;
        if(SolutionCounter < ToConvertNumber * 16) ToConvertNumber = SolutionCounter;
        --Index1;
        break;
  }}}

  /* output */
  x = strlen(SolutionPDB);
  printf("\nREPORT :\n\n");
  ReportOut = fopen(Report, "wb");
  sprintf(Line, "Map              : %s\n", DensityMap);
  printf(Line);
  fwrite(&Line, 1, strlen(Line), ReportOut);
  sprintf(Line, " Density cut-off : %f\n", CutOff);
  printf(Line);
  fwrite(&Line, 1, strlen(Line), ReportOut);
  sprintf(Line, " Voxel size      : %f Angstrom\n", VoxelSize);
  printf(Line);
  fwrite(&Line, 1, strlen(Line), ReportOut);
  sprintf(Line, " Resolution      : %f Angstrom\n\n", Resolution);
  printf(Line);
  fwrite(&Line, 1, strlen(Line), ReportOut);
  sprintf(Line, "Model            : %s\n\n", PdbFile);
  printf(Line);
  fwrite(&Line, 1, strlen(Line), ReportOut);
  sprintf(Line, "Minimal surface overlap      : %5.2f %%\n", MinSurfaceOverlap * 100);
  printf(Line);
  fwrite(&Line, 1, strlen(Line), ReportOut);
  sprintf(Line, "Number of solutions to score : %d\n\n", ToConvertNumber);
  printf(Line);
  fwrite(&Line, 1, strlen(Line), ReportOut);
  for(Index1 = 0; Index1 < ToConvertNumber; Index1++) {
    ParameterPointer = Solutions + (SolutionCounter - Index1 * 16);
    Rc11 = *(ParameterPointer - 12);
    Rc12 = *(ParameterPointer - 11);
    Rc13 = *(ParameterPointer - 10);
    Rc21 = *(ParameterPointer - 9);
    Rc22 = *(ParameterPointer - 8);
    Rc23 = *(ParameterPointer - 7);
    Rc31 = *(ParameterPointer - 6);
    Rc32 = *(ParameterPointer - 5);
    Rc33 = *(ParameterPointer - 4);
    /* reset the origin to the center of the map */
    TcX = (*(ParameterPointer - 3) + Rc11 + Rc12 + Rc13 - (ColumnD / 2 + 0.5)) * VoxelSize -
      (Rc11 * MinX + Rc12 * MinY + Rc13 * MinZ);
    TcY = (*(ParameterPointer - 2) + Rc21 + Rc22 + Rc23 - (RowD / 2 + 0.5)) * VoxelSize -
      (Rc21 * MinX + Rc22 * MinY + Rc23 * MinZ);
    TcZ = (*(ParameterPointer - 1) + Rc31 + Rc32 + Rc33 - (SectionD / 2 + 0.5)) * VoxelSize -
      (Rc31 * MinX + Rc32 * MinY + Rc33 * MinZ);
    sprintf(Line, "Score : % 12g, Overlap : % 12g, Direct CC : % 12g, Laplacian CC : % 12g\n", *(ParameterPointer - 16), *(ParameterPointer - 15), *(ParameterPointer - 14), *(ParameterPointer - 13));
    printf(Line);
    fwrite(&Line, 1, strlen(Line), ReportOut);
    sprintf(Line, "Rotation    : % 12g % 12g % 12g\n", Rc11 , Rc12, Rc13);
    printf(Line);
    fwrite(&Line, 1, strlen(Line), ReportOut);
    sprintf(Line, "              % 12g % 12g % 12g\n", Rc21, Rc22, Rc23);
    printf(Line);
    fwrite(&Line, 1, strlen(Line), ReportOut);
    sprintf(Line, "              % 12g % 12g % 12g\n", Rc31, Rc32, Rc33);
    printf(Line);
    fwrite(&Line, 1, strlen(Line), ReportOut);
    sprintf(Line, "Translation : % 12g % 12g % 12g\n\n", TcX, TcY, TcZ);
    printf(Line);
    fwrite(&Line, 1, strlen(Line), ReportOut);

    /* rotate and translate the model */
    sprintf(SolutionPDB + x, "%d", Index1 + 1);
    strcat(SolutionPDB, ".pdb");
    Model = fopen(PdbFile, "r");
    ModelOut = fopen(SolutionPDB, "wb");
    if(IncludeMap) {
      Map = fopen(MapPDB, "r");
      ExtremaCounter = 101; /* read/written byte number */
      while(ExtremaCounter == 101) {
        ExtremaCounter = fread(&Line, 1, 101, Map);
        fwrite(&Line, 1, ExtremaCounter, ModelOut);
      } fclose(Map);
    } SixFirstOfLine[6] = '\0';
    Coordinates[24] = '\0';
    while(fgets(Line, sizeof(Line), Model)) {
      memcpy(SixFirstOfLine, Line, 6);
      if(!(strcmp(SixFirstOfLine, "ATOM  ")&&strcmp(SixFirstOfLine, "HETATM"))) {
        memcpy(Coordinates, Line + 30, 24);
        sscanf(Coordinates, "%8f%8f%8f", &X, &Y, &Z);
        Z2 = Rc33 * Z + Rc32 * Y + Rc31 * X + TcZ;
        Y2 = Rc23 * Z + Rc22 * Y + Rc21 * X + TcY;
        X2 = Rc13 * Z + Rc12 * Y + Rc11 * X + TcX;
        sprintf(Coordinates, "%8.3f%8.3f%8.3f", X2, Y2, Z2);
        memcpy(Line + 30, Coordinates, 24);
      } fwrite(&Line, 1, strlen(Line), ModelOut);         
    } fclose(Model);
    fclose(ModelOut);
    SolutionPDB[x] = '\0';
  } fclose(ReportOut);
  return(0);
}



















