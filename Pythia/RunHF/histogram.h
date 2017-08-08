
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
  Float_t ePosParentPt;
  Float_t ePosParentEta;
  Float_t ePosParentPhi;
  Float_t ePosPt;
  Float_t ePosEta;
  Float_t ePosPhi;
  Int_t   eNegParentGID;
  Float_t eNegParentPt;
  Float_t eNegParentEta;
  Float_t eNegParentPhi;
  Float_t eNegPt;
  Float_t eNegEta;
  Float_t eNegPhi;


  //Float_t ePaPosPt;
  //Float_t ePaPosEta;
  //Float_t ePaPosPhi;
  //Float_t ePaPosY;
  //Float_t ePaNegPt;
  //Float_t ePaNegEta;
  //Float_t ePaNegPhi;
  //Float_t ePaNegY;

}; trkTree meTree;
