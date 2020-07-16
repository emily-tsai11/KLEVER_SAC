// CustomMuonDecay.cc
// ------------------------------------------------------------
// *** WARNING: this file is currently just a place holder!!!! ***
// *** It currently reproduces the G4MuonDecayChannelWithSpin class.
// *** The aim of this file is to introduce a new mu -> e nu nu (gamma)
// *** generator, which is not available yet.
// ------------- G4MuonDecayChannelWithSpin info --------------
// GEANT 4 class header file
//
//		17 August 2004 P. Gumplinger and T. MacPhail
//		samples Michel spectrum including 1st order
//		radiative corrections
//			Reference: Florian Scheck "Muon Physics", in Physics Reports
//			(Review Section of Physics Letters) 44, No. 4 (1978)
//			187-248. North-Holland Publishing Company, Amsterdam
//			at page 210 cc.
//
//			W.E. Fisher and F. Scheck, Nucl. Phys. B83 (1974) 25.
// ------------------------------------------------------------
// History:
//
// Created by Karim Massri (karim.massri@cern.ch) 2017-10-09
//		based on G4MuonDecayChannelWithSpin class (see info above)
// Adapted from KLMC by Emily Tsai (emily.tsai11@gmail.com) 2020-7-10
// --------------------------------------------------------------

#include "CustomMuonDecay.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4DecayProducts.hh"
#include "G4LorentzVector.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CustomMuonDecay::CustomMuonDecay() : G4MuonDecayChannel(), parent_polarization(), EMMU(0.0 * MeV), EMASS(0.0 * MeV) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CustomMuonDecay::CustomMuonDecay(const G4String& theParentName, G4double theBR) : G4MuonDecayChannel(theParentName, theBR), parent_polarization(), EMMU(0.0 * MeV), EMASS(0.0 * MeV) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CustomMuonDecay::~CustomMuonDecay() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CustomMuonDecay::CustomMuonDecay(const CustomMuonDecay& right) : G4MuonDecayChannel(right)
{
	parent_polarization = right.parent_polarization;
	EMMU = right.EMMU;
	EMASS = right.EMASS;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CustomMuonDecay& CustomMuonDecay::operator=(const CustomMuonDecay& right)
{
	if(this != &right)
	{
		kinematics_name = right.kinematics_name;
		verboseLevel = right.verboseLevel;
		rbranch = right.rbranch;

		// copy parent name
		parent_name = new G4String(*right.parent_name);

		// clear daughters_name array
		ClearDaughtersName();

		// recreate array
		numberOfDaughters = right.numberOfDaughters;
		if(numberOfDaughters > 0)
		{
			if(daughters_name != 0) ClearDaughtersName();
			daughters_name = new G4String*[numberOfDaughters];
			//copy daughters name
			for(G4int index = 0; index < numberOfDaughters; index++)
			{
				daughters_name[index] = new G4String(*right.daughters_name[index]);
			}
		}
		parent_polarization = right.parent_polarization;
		EMMU  = right.EMMU;
		EMASS = right.EMASS;
	}
	return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// this version assumes V-A coupling with 1st order radiative correctons,
//		the standard model Michel parameter values, but
//		gives incorrect energy spectrum for neutrinos.
G4DecayProducts *CustomMuonDecay::DecayIt(G4double)
{
#ifdef G4VERBOSE
	if(GetVerboseLevel() > 1) G4cout << "CustomMuonDecay::DecayIt ";
#endif

	// parent mass
	G4double parentmass = GetParent()->GetPDGMass();

	EMMU = parentmass;

	//daughters' mass
	G4double daughtermass[3];
	G4double sumofdaughtermass = 0.0;
	for(G4int index = 0; index < 3; index++)
	{
		daughtermass[index] = GetDaughter(index)->GetPDGMass();
		sumofdaughtermass += daughtermass[index];
	}

	EMASS = daughtermass[0];

	// create parent G4DynamicParticle at rest
	G4ThreeVector dummy;
	G4DynamicParticle* parentparticle = new G4DynamicParticle(GetParent(), dummy, 0.0);
	//create G4Decayproducts
	G4DecayProducts* products = new G4DecayProducts(*parentparticle);
	delete parentparticle;

	// calculate electron energy
	G4double michel_rho = 0.75;		// Standard Model Michel rho
	G4double michel_delta = 0.75;	// Standard Model Michel delta
	G4double michel_xsi = 1.00;		// Standard Model Michel xsi
	G4double michel_eta = 0.00;		// Standard Model eta

	G4double rndm, x, ctheta;

	G4double FG;
	G4double FG_max = 2.00;

	G4double W_mue = (EMMU * EMMU + EMASS * EMASS) / (2.0 * EMMU);
	G4double x0 = EMASS / W_mue;

	G4double x0_squared = x0 * x0;

	// ****************************************************
	// x0 <= x <= 1.0   and   -1 <= y <= 1
	//
	// F(x, y) = f(x) * g(x, y);   g(x, y) = 1.0 + g(x) * y
	// ****************************************************

	// sampling F(x, y) directly (brute force)
	do
	{
		// sample the positron energy by sampling from F
		rndm = G4UniformRand();

		x = x0 + rndm * (1.0 - x0);

		G4double x_squared = x * x;

		G4double F_IS, F_AS, G_IS, G_AS;

		F_IS = 1.0 / 6.0 * (-2.0 * x_squared + 3.0 * x - x0_squared);
		F_AS = 1.0 / 6.0 * std::sqrt(x_squared - x0_squared) * (2.0 * x - 2.0 + std::sqrt(1.0 - x0_squared));

		G_IS = 2.0 / 9.0 * (michel_rho - 0.75) * (4.0 * x_squared - 3.0 * x - x0_squared);
		G_IS = G_IS + michel_eta * (1.0 - x) * x0;

		G_AS = 3.0 * (michel_xsi - 1.0) * (1.0 - x);
		G_AS = G_AS + 2.0 * (michel_xsi * michel_delta - 0.75) * (4.0 * x - 4.0 + std::sqrt(1.0 - x0_squared));
		G_AS = 1.0 / 9.0 * std::sqrt(x_squared - x0_squared) * G_AS;

		F_IS = F_IS + G_IS;
		F_AS = F_AS + G_AS;

		// radiative corrections
		G4double R_IS = F_c(x, x0);

		G4double F = 6.0 * F_IS + R_IS / std::sqrt(x_squared - x0_squared);

		// radiative corrections
		G4double R_AS = F_theta(x, x0);

		rndm = G4UniformRand();

		ctheta = 2.0 * rndm - 1.0;

		G4double G = 6.0 * F_AS - R_AS / std::sqrt(x_squared - x0_squared);

		FG = std::sqrt(x_squared - x0_squared) * F * (1.0 + (G / F) * ctheta);

		if(FG > FG_max)
		{
			G4cout << "***Problem in Muon Decay *** : FG > FG_max" << G4endl;
			FG_max = FG;
		}

		rndm = G4UniformRand();
	}
	while(FG < rndm * FG_max);

	G4double energy = x * W_mue;

	rndm = G4UniformRand();

	G4double phi = twopi * rndm;

	if(energy < EMASS) energy = EMASS;

	// calculate daughter momentum
	G4double daughtermomentum[3];

	daughtermomentum[0] = std::sqrt(energy * energy - EMASS * EMASS);

	G4double stheta = std::sqrt(1.0 - ctheta * ctheta);
	G4double cphi = std::cos(phi);
	G4double sphi = std::sin(phi);

	// coordinates of the decay positron with respect to the muon spin
	G4double px = stheta * cphi;
	G4double py = stheta * sphi;
	G4double pz = ctheta;

	G4ThreeVector direction0(px, py, pz);

	direction0.rotateUz(parent_polarization);

	G4DynamicParticle* daughterparticle0 = new G4DynamicParticle(GetDaughter(0), daughtermomentum[0] * direction0);

	products->PushProducts(daughterparticle0);

	// daughter 1, 2 (neutrinos)
	// create neutrinos in the C.M frame of two neutrinos
	G4double energy2 = parentmass * (1.0 - x / 2.0);
	G4double vmass = std::sqrt((energy2 - daughtermomentum[0]) * (energy2 + daughtermomentum[0]));
	G4double beta = -1.0 * daughtermomentum[0] / energy2;
	G4double costhetan = 2.0 * G4UniformRand() - 1.0;
	G4double sinthetan = std::sqrt((1.0 - costhetan) * (1.0 + costhetan));
	G4double phin = twopi * G4UniformRand() * rad;
	G4double sinphin = std::sin(phin);
	G4double cosphin = std::cos(phin);

	G4ThreeVector direction1(sinthetan * cosphin, sinthetan * sinphin, costhetan);
	G4DynamicParticle* daughterparticle1 = new G4DynamicParticle(GetDaughter(1), direction1 * (vmass / 2.0));
	G4DynamicParticle* daughterparticle2 = new G4DynamicParticle(GetDaughter(2), direction1 * (-1.0 * vmass / 2.0));

	// boost to the muon rest frame
	G4LorentzVector p4;
	p4 = daughterparticle1->Get4Momentum();
	p4.boost(direction0.x() * beta, direction0.y() * beta, direction0.z() * beta);
	daughterparticle1->Set4Momentum(p4);
	p4 = daughterparticle2->Get4Momentum();
	p4.boost(direction0.x() * beta, direction0.y() * beta, direction0.z() * beta);
	daughterparticle2->Set4Momentum(p4);
	products->PushProducts(daughterparticle1);
	products->PushProducts(daughterparticle2);
	daughtermomentum[1] = daughterparticle1->GetTotalMomentum();
	daughtermomentum[2] = daughterparticle2->GetTotalMomentum();

#ifdef G4VERBOSE
	// output message
	if(GetVerboseLevel() > 1)
	{
		G4cout << "CustomMuonDecay::DecayIt ";
		G4cout << "  create decay products in rest frame " << G4endl;
		products->DumpInfo();
	}
#endif

	return products;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double CustomMuonDecay::R_c(G4double x)
{
	G4int n_max = (int) (100.0 * x);

	if(n_max < 10) n_max = 10;

	G4double L2 = 0.0;

	for(G4int n = 1; n <= n_max; n++) L2 += std::pow(x, n) / (n * n);

	G4double omega = std::log(EMMU / EMASS);

	G4double r_c;

	r_c = 2.0 * L2 - (pi * pi / 3.0) - 2.0;
	r_c = r_c + omega * (1.5 + 2.0 * std::log((1.0 - x) / x));
	r_c = r_c - std::log(x) * (2.0 * std::log(x) - 1.0);
	r_c = r_c + (3.0 * std::log(x) - 1.0 - 1.0 / x) * std::log(1.0 - x);

	return r_c;
}
