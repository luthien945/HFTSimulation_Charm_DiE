
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
  Float_t ePosVx;
  Float_t ePosVy;
  Float_t ePosVz;

  Int_t   eNegParentGID;
  Float_t eNegParentPt;
  Float_t eNegParentEta;
  Float_t eNegParentPhi;
  Float_t eNegPt;
  Float_t eNegEta;
  Float_t eNegPhi;
  Float_t eNegVx;
  Float_t eNegVy;
  Float_t eNegVz;

}; trkTree meTree;
