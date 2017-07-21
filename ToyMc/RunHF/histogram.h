
TTree *eTree;

struct trkTree
{
  Int_t   EventId;

  Float_t cPt;
  Float_t cEta;
  Float_t cPhi;
  Float_t cY;
  
  Float_t cbarPt;
  Float_t cbarEta;
  Float_t cbarPhi;
  Float_t cbarY;

  Float_t eePt;
  Float_t eeEta;
  Float_t eePhi;
  Float_t eeRapidity;
  Float_t eeM;
  Int_t   ePosParentGID;
  Float_t ePosPt;
  Float_t ePosEta;
  Float_t ePosPhi;
  Float_t ePosX;
  Float_t ePosY;
  Float_t ePosZ;
  Int_t   eNegParentGID;
  Float_t eNegPt;
  Float_t eNegEta;
  Float_t eNegPhi;
  Float_t eNegX;
  Float_t eNegY;
  Float_t eNegZ;


  //Float_t ePaPosPt;
  //Float_t ePaPosEta;
  //Float_t ePaPosPhi;
  //Float_t ePaPosY;
  //Float_t ePaNegPt;
  //Float_t ePaNegEta;
  //Float_t ePaNegPhi;
  //Float_t ePaNegY;

}; trkTree meTree;
