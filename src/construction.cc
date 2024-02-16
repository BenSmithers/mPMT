/*                                                                                                                        
Author:    Mohit Gola 10th July 2023                                                                                       
*/


#include "construction.hh"

myDetectorConstruction* myDetectorConstruction::instance = NULL;

myDetectorConstruction::myDetectorConstruction(G4int DetConfig, WCSimTuningParameters* WCSimTuningPars) : WCSimTuningParams(WCSimTuningPars)
{
  myConfiguration = DetConfig;
  instance = this;
}

myDetectorConstruction::~myDetectorConstruction()
{}

//G4VPhysicalVolume *myDetectorConstruction::Construct()
G4LogicalVolume *myDetectorConstruction::ConstructPMT()
{

  G4NistManager *nist = G4NistManager::Instance();
  G4Material *Air = nist->FindOrBuildMaterial("G4_AIR");

  /////////////////////vacuum///////////////

  density     = universe_mean_density;              //from PhysicalConstants.h                                        
  G4double pressure    = 1.e-19*pascal;
  G4double temperature = 0.1*kelvin;
  a = 1.01*g/mole;
  G4Material* Vacuum =
    new G4Material("Vacuum", 1., a, density,
                   kStateGas,temperature,pressure);
  ////////////AIR//////////////////////  
  a = 14.01*g/mole;
  G4Element* elN
    = new G4Element("Nitrogen","N", 7,a);

  a = 16.00*g/mole;
  G4Element* elO
  = new G4Element("Oxygen","O", 8,a);

  density = 1.290*mg/cm3;
  G4Material* Air1
    = new G4Material("Air",density,2);
  Air1->AddElement(elN, 70.*perCent);
  Air1->AddElement(elO, 30.*perCent);

  //////////////Water///////////////////

  a = 1.01*g/mole;
  G4Element* H
    = new G4Element("Hydrogen","H", 1,a);

  a = 16.00*g/mole;
  G4Element* O
    = new G4Element("Oxygen","O", 8,a);

  density = 1.00*g/cm3;
  G4Material* Water
    = new G4Material("Water",density,2);
  Water->AddElement(H, 2);
  Water->AddElement(O, 1);

  /////////////////////Aluminum///////////////////                                                                          
  a = 26.98*g/mole;
  G4Element* elAl = new G4Element("Aluminum", "Al", 13, a);

  density = 2.7*g/cm3;
  G4Material* Aluminum
    = new G4Material("Aluminum",density,1);
  Aluminum->AddElement(elAl, 1);


  /////////////Absorber////////////////

  G4double absorberThickness = 1.0*mm;
  G4double absorberheight = absorberThickness/2.;
  density = 0.96*g/cm3;

  G4Element *elC = new G4Element("Carbon", "C", 6, 12.01*g/mole);
  G4Element *elH = new G4Element("Hydrogen", "H", 1, 1.01*g/mole);
  G4Material *absorberMaterial = new G4Material("customAbsorber", density, 2);

  absorberMaterial->AddElement(elC, 1);
  absorberMaterial->AddElement(elH, 4);

  G4double absorptionCoeff = 1.0e6 / cm;


  const G4int NUMENTRIES_ref = 501;

  G4double ENERGY_ref[NUMENTRIES_ref] = { 1.7712e-09, 1.77374e-09, 1.77628e-09, 1.77883e-09, 1.78138e-09, 1.78395e-09,
                                          1.78652e-09, 1.78909e-09, 1.79168e-09, 1.79427e-09, 1.79687e-09, 1.79948e-09,
                                          1.8021e-09, 1.80472e-09, 1.80735e-09, 1.80999e-09, 1.81263e-09, 1.81529e-09,
                                          1.81795e-09, 1.82062e-09, 1.8233e-09, 1.82598e-09, 1.82868e-09, 1.83138e-09,
                                          1.83409e-09, 1.8368e-09, 1.83953e-09, 1.84226e-09, 1.845e-09, 1.84775e-09,
                                          1.85051e-09, 1.85328e-09, 1.85605e-09, 1.85883e-09, 1.86162e-09, 1.86442e-09,
                                          1.86723e-09, 1.87005e-09, 1.87287e-09, 1.87571e-09, 1.87855e-09, 1.8814e-09,
                                          1.88426e-09, 1.88713e-09, 1.89e-09, 1.89289e-09, 1.89578e-09, 1.89869e-09,
                                          1.9016e-09, 1.90452e-09, 1.90745e-09, 1.91039e-09, 1.91334e-09, 1.91629e-09,
                                          1.91926e-09, 1.92224e-09, 1.92522e-09, 1.92821e-09, 1.93122e-09, 1.93423e-09,
                                          1.93725e-09, 1.94028e-09, 1.94333e-09, 1.94638e-09, 1.94944e-09, 1.95251e-09,
                                          1.95559e-09, 1.95868e-09, 1.96178e-09, 1.96488e-09, 1.968e-09, 1.97113e-09,
                                          1.97427e-09, 1.97742e-09, 1.98058e-09, 1.98375e-09, 1.98693e-09, 1.99012e-09,
                                          1.99332e-09, 1.99652e-09, 1.99975e-09, 2.00298e-09, 2.00622e-09, 2.00947e-09,
                                          2.01273e-09, 2.016e-09, 2.01929e-09, 2.02258e-09, 2.02589e-09, 2.0292e-09,
                                          2.03253e-09, 2.03587e-09, 2.03921e-09, 2.04257e-09, 2.04594e-09, 2.04933e-09,
                                          2.05272e-09, 2.05612e-09, 2.05954e-09, 2.06297e-09, 2.0664e-09, 2.06985e-09,
                                          2.07331e-09, 2.07679e-09, 2.08027e-09, 2.08377e-09, 2.08728e-09, 2.0908e-09,
                                          2.09433e-09, 2.09787e-09, 2.10143e-09, 2.10499e-09, 2.10857e-09, 2.11217e-09,
                                          2.11577e-09, 2.11939e-09, 2.12302e-09, 2.12666e-09, 2.13031e-09, 2.13398e-09,
                                          2.13766e-09, 2.14135e-09, 2.14506e-09, 2.14877e-09, 2.1525e-09, 2.15625e-09,
                                          2.16e-09, 2.16377e-09, 2.16756e-09, 2.17135e-09, 2.17516e-09, 2.17898e-09,
					  2.18282e-09, 2.18667e-09, 2.19053e-09, 2.19441e-09, 2.1983e-09, 2.20221e-09,
                                          2.20612e-09, 2.21006e-09, 2.214e-09, 2.21796e-09, 2.22194e-09, 2.22593e-09,
                                          2.22993e-09, 2.23395e-09, 2.23798e-09, 2.24203e-09, 2.24609e-09, 2.25017e-09,
                                          2.25426e-09, 2.25836e-09, 2.26249e-09, 2.26662e-09, 2.27077e-09, 2.27494e-09,
                                          2.27912e-09, 2.28332e-09, 2.28753e-09, 2.29176e-09, 2.296e-09, 2.30026e-09,
                                          2.30454e-09, 2.30883e-09, 2.31314e-09, 2.31746e-09, 2.3218e-09, 2.32616e-09,
                                          2.33053e-09, 2.33492e-09, 2.33932e-09, 2.34375e-09, 2.34819e-09,
                                          2.35264e-09, 2.35711e-09, 2.3616e-09, 2.36611e-09, 2.37063e-09, 2.37518e-09,
                                          2.37974e-09, 2.38431e-09, 2.38891e-09, 2.39352e-09, 2.39815e-09, 2.40279e-09,
                                          2.40746e-09, 2.41214e-09, 2.41685e-09, 2.42157e-09, 2.42631e-09, 2.43106e-09,
                                          2.43584e-09, 2.44063e-09, 2.44545e-09, 2.45028e-09, 2.45513e-09, 2.46e-09,
                                          2.46489e-09, 2.4698e-09, 2.47473e-09, 2.47968e-09, 2.48465e-09, 2.48964e-09,
                                          2.49465e-09, 2.49968e-09, 2.50473e-09, 2.5098e-09, 2.51489e-09, 2.52e-09,
                                          2.52514e-09, 2.53029e-09, 2.53546e-09, 2.54066e-09, 2.54588e-09, 2.55112e-09,
                                          2.55638e-09, 2.56166e-09, 2.56696e-09, 2.57229e-09, 2.57763e-09, 2.583e-09,
                                          2.5884e-09, 2.59381e-09, 2.59925e-09, 2.60471e-09, 2.61019e-09, 2.6157e-09,
                                          2.62123e-09, 2.62678e-09, 2.63236e-09, 2.63796e-09, 2.64359e-09, 2.64924e-09,
					  2.65491e-09, 2.66061e-09, 2.66633e-09, 2.67207e-09, 2.67784e-09, 2.68364e-09,
                                          2.68946e-09, 2.69531e-09, 2.70118e-09, 2.70708e-09, 2.713e-09, 2.71895e-09,
                                          2.72493e-09, 2.73093e-09, 2.73696e-09, 2.74301e-09, 2.7491e-09, 2.7552e-09,
                                          2.76134e-09, 2.7675e-09, 2.7737e-09, 2.77991e-09, 2.78616e-09, 2.79244e-09,
                                          2.79874e-09, 2.80507e-09, 2.81143e-09, 2.81782e-09, 2.82424e-09, 2.83069e-09,
                                          2.83717e-09, 2.84367e-09, 2.85021e-09, 2.85678e-09, 2.86338e-09, 2.87e-09,
                                          2.87666e-09, 2.88335e-09, 2.89007e-09, 2.89683e-09, 2.90361e-09, 2.91043e-09,
                                          2.91728e-09, 2.92416e-09, 2.93107e-09, 2.93801e-09, 2.94499e-09, 2.952e-09,
                                          2.95905e-09, 2.96613e-09, 2.97324e-09, 2.98039e-09, 2.98757e-09, 2.99479e-09,
                                          3.00204e-09, 3.00933e-09, 3.01665e-09, 3.024e-09, 3.0314e-09, 3.03883e-09,
                                          3.04629e-09, 3.0538e-09, 3.06134e-09, 3.06892e-09, 3.07653e-09, 3.08418e-09,
                                          3.09188e-09, 3.0996e-09, 3.10737e-09, 3.11518e-09, 3.12303e-09, 3.13091e-09,
                                          3.13884e-09, 3.14681e-09, 3.15481e-09, 3.16286e-09, 3.17095e-09, 3.17908e-09,
                                          3.18725e-09, 3.19547e-09, 3.20373e-09, 3.21203e-09, 3.22037e-09, 3.22876e-09,
                                          3.23719e-09, 3.24566e-09, 3.25418e-09, 3.26274e-09, 3.27135e-09, 3.28001e-09,
                                          3.28871e-09, 3.29745e-09, 3.30625e-09, 3.31509e-09, 3.32397e-09, 3.33291e-09,
                                          3.34189e-09, 3.35092e-09, 3.36001e-09, 3.36914e-09, 3.37832e-09, 3.38755e-09,
					  3.39683e-09, 3.40616e-09, 3.41554e-09, 3.42498e-09, 3.43447e-09, 3.44401e-09,
                                          3.4536e-09, 3.46325e-09, 3.47295e-09, 3.4827e-09, 3.49251e-09, 3.50238e-09,
                                          3.5123e-09, 3.52228e-09, 3.53231e-09, 3.54241e-09, 3.55256e-09, 3.56276e-09,
                                          3.57303e-09, 3.58336e-09, 3.59374e-09, 3.60419e-09, 3.6147e-09, 3.62527e-09,
                                          3.6359e-09, 3.64659e-09, 3.65735e-09, 3.66817e-09, 3.67906e-09, 3.69001e-09,
                                          3.70102e-09, 3.7121e-09, 3.72325e-09, 3.73446e-09, 3.74575e-09, 3.7571e-09,
                                          3.76852e-09, 3.78001e-09, 3.79157e-09, 3.8032e-09, 3.8149e-09, 3.82667e-09,
                                          3.83852e-09, 3.85044e-09, 3.86244e-09, 3.87451e-09, 3.88665e-09, 3.89887e-09,
                                          3.91117e-09, 3.92355e-09, 3.93601e-09, 3.94854e-09, 3.96116e-09, 3.97385e-09,
                                          3.98663e-09, 3.99949e-09, 4.01243e-09, 4.02546e-09, 4.03857e-09, 4.05177e-09,
                                          4.06506e-09, 4.07843e-09, 4.09189e-09, 4.10544e-09, 4.11908e-09, 4.13281e-09,
                                          4.14663e-09, 4.16054e-09, 4.17455e-09, 4.18866e-09, 4.20285e-09, 4.21715e-09,
                                          4.23154e-09, 4.24603e-09, 4.26063e-09, 4.27532e-09, 4.29011e-09, 4.30501e-09,
                                          4.32001e-09, 4.33511e-09, 4.35032e-09, 4.36564e-09, 4.38107e-09, 4.3966e-09,
                                          4.41225e-09, 4.42801e-09, 4.44388e-09, 4.45986e-09, 4.47596e-09, 4.49218e-09,
                                          4.50852e-09, 4.52497e-09, 4.54155e-09, 4.55824e-09, 4.57506e-09, 4.59201e-09,
                                          4.60908e-09, 4.62628e-09, 4.6436e-09, 4.66106e-09, 4.67865e-09, 4.69637e-09,
					  4.71423e-09, 4.73222e-09, 4.75035e-09, 4.76862e-09, 4.78703e-09, 4.80559e-09,
                                          4.82429e-09, 4.84313e-09, 4.86213e-09, 4.88127e-09, 4.90056e-09, 4.92001e-09,
                                          4.93961e-09, 4.95937e-09, 4.97929e-09, 4.99936e-09, 5.0196e-09, 5.04001e-09,
                                          5.06058e-09, 5.08132e-09, 5.10223e-09, 5.12331e-09, 5.14457e-09, 5.16601e-09,
                                          5.18762e-09, 5.20942e-09, 5.2314e-09, 5.25357e-09, 5.27592e-09, 5.29847e-09,
                                          5.32121e-09, 5.34415e-09, 5.36728e-09, 5.39062e-09, 5.41416e-09, 5.4379e-09,
                                          5.46186e-09, 5.48603e-09, 5.51041e-09, 5.53501e-09, 5.55983e-09, 5.58487e-09,
                                          5.61014e-09, 5.63565e-09, 5.66138e-09, 5.68735e-09, 5.71356e-09, 5.74001e-09,
                                          5.76671e-09, 5.79365e-09, 5.82085e-09, 5.84831e-09, 5.87603e-09, 5.90401e-09,
                                          5.93226e-09, 5.96078e-09, 5.98957e-09, 6.01865e-09, 6.04801e-09, 6.07766e-09,
                                          6.1076e-09, 6.13783e-09, 6.16837e-09, 6.19921e-09 };


  G4double REFLECTIVITY_ref[NUMENTRIES_ref] = { 91.867018, 91.878821, 91.909624, 91.933169, 91.975042, 91.994669,
                                                92.023845, 92.05233, 92.04163, 92.116707, 92.116159, 92.172474, 92.190017,
                                                92.18805, 92.234936, 92.254988, 92.28962, 92.306398, 92.334575, 92.357688,
                                                92.381986, 92.396137, 92.430164, 92.448784, 92.465907, 92.492628,
                                                92.514583, 92.564694, 92.586161, 92.585568, 92.609252, 92.623107,
                                                92.653067, 92.670681, 92.69944, 92.71874, 92.74472, 92.735985, 92.774786,
                                                92.797425, 92.77678, 92.83881, 92.882547, 92.886619, 92.880202, 92.926185,
                                                92.91282, 92.941198, 92.97765, 92.971139, 93.010272, 93.005497, 93.035958,
                                                93.064111, 93.085596, 93.108483, 93.1252, 93.113579, 93.144708, 93.159892,
                                                93.169539, 93.167694, 93.210855, 93.221407, 93.229929, 93.217626,
                                                93.261842, 93.267996, 93.28614, 93.305264, 93.279379, 93.313851, 93.334877,
                                                93.357556, 93.384519, 93.398249, 93.397436, 93.41664, 93.426486, 93.431823,
                                                93.442491, 93.463716, 93.500783, 93.516479, 93.504207, 93.518962,
                                                93.530918, 93.544686, 93.560372, 93.582261, 93.591123, 93.591639,
                                                93.596435, 93.605721, 93.621836, 93.639598, 93.625838, 93.65027, 93.679588,
                                                93.701544, 93.727586, 93.736008, 93.731122, 93.730506, 93.732266,
                                                93.737014, 93.732351, 93.745206, 93.774816, 93.792166, 93.81091, 93.814387,
                                                93.829906, 93.835502, 93.849515, 93.874573, 93.87148, 93.837158, 93.857773,
                                                93.88873, 93.908168, 93.917556, 93.931774, 93.956124, 93.966123, 93.963233,
                                                93.975599, 93.985593, 93.976342, 93.978551, 94.003286, 94.02512, 94.037606,
                                                94.037748, 94.046234, 94.061184, 94.085374, 94.105208, 94.130384,
                                                94.116635, 94.085321, 94.089851, 94.08085, 94.080903, 94.106384, 94.120388,
                                                94.128558, 94.128922, 94.135115, 94.1421, 94.151582, 94.165153, 94.178154,
                                                94.191268, 94.201176, 94.197218, 94.199962, 94.206345, 94.217231,
						94.216899, 94.229288, 94.235705, 94.232253, 94.245712, 94.255995,
                                                94.256122, 94.258842, 94.26191, 94.280816, 94.29058, 94.279921, 94.27562,
                                                94.297274, 94.310444, 94.316056, 94.319721, 94.326782, 94.322525,
                                                94.329357, 94.328025, 94.330324, 94.323669, 94.327122, 94.343237,
                                                94.336868, 94.333659, 94.338033, 94.356735, 94.368169, 94.355407,
                                                94.343574, 94.340561, 94.336776, 94.315306, 94.281768, 94.247779,
                                                94.216414, 94.205492, 94.147018, 94.064085, 93.994456, 93.916984,
                                                93.853796, 93.763506, 93.648232, 93.582235, 93.517931, 93.426367,
                                                93.344305, 93.276234, 93.235772, 93.186943, 93.167268, 93.151248,
                                                93.126298, 93.117566, 93.152638, 93.196265, 93.244511, 93.297256,
                                                93.351183, 93.386929, 93.437783, 93.48046, 93.489035, 93.494792,
                                                93.507843, 93.521117, 93.50747, 93.552294, 93.569038, 93.554846,
                                                93.54536, 93.516454, 93.517806, 93.50129, 93.488663, 93.41786, 93.334727,
                                                93.31322, 93.240218, 93.156117, 93.03463, 92.941609, 92.819197, 92.743487,
                                                92.660401, 92.548155, 92.433372, 92.295113, 92.23734, 92.134051, 92.041279,
                                                91.948048, 91.850696, 91.834416, 91.77905, 91.734713, 91.697977, 91.68431,
                                                91.645102, 91.564356, 91.502549, 91.471914, 91.375934, 91.319727, 91.25141,
						91.194892, 91.117668, 91.025524, 90.978327, 90.90553, 90.788165, 90.687478,
                                                90.599473, 90.530206, 90.407009, 90.329717, 90.225505, 90.110879,
                                                90.013234, 89.890837, 89.790649, 89.661338, 89.520829, 89.373676,
                                                89.26066, 89.094177, 88.966278, 88.799792, 88.655118, 88.49559, 88.305646,
                                                88.077914, 87.912389, 87.69281, 87.467818, 87.275388, 87.031673, 86.773113,
                                                86.518416, 86.211943, 85.960268, 85.638584, 85.344267, 85.031389,
                                                84.651235, 84.253471, 83.843647, 83.438015, 82.99535, 82.52885, 82.069422,
                                                81.525201, 80.920414, 80.305407, 79.638057, 78.943532, 78.211256,
                                                77.434004, 76.559918, 75.566542, 73.961647, 72.914792, 71.985872,
                                                71.870066, 70.170831, 67.919187, 66.317085, 64.606509, 62.820679,
                                                60.919403, 58.910818, 56.816265, 54.628183, 52.432657, 50.238826,
                                                48.064867, 46.011777, 44.105821, 42.452535, 41.061547, 39.954065,
                                                39.17797, 38.732376, 38.664667, 38.903143, 39.417775, 40.172204,
                                                41.096687, 42.153995, 43.298425, 44.4908, 45.674237, 46.857062, 47.960048,
                                                49.011047, 50.008499, 50.912678, 51.714986, 52.463473, 53.124773,
                                                53.694391, 54.214078, 54.641626, 54.989054, 55.281175, 55.503411,
                                                55.669049, 55.809089, 55.879853, 55.877649, 55.851489, 55.789731,
						55.664089, 55.494965, 55.270052, 55.014417, 54.72109, 54.343811,
                                                53.96432, 53.571113, 53.331219, 53.202828, 52.733801, 52.234025, 51.71219,
                                                51.148939, 50.605254, 50.016304, 49.404896, 48.785501, 48.143283,
                                                47.467895, 46.746983, 46.018764, 45.290908, 44.542634, 43.792217, 43.04205,
                                                42.264221, 41.504135, 40.76822, 40.020852, 39.292485, 38.603477, 37.930307,
                                                37.291234, 36.670246, 36.064879, 35.528691, 35.01708, 34.529849, 34.082121,
                                                33.655095, 33.245391, 32.863984, 32.495056, 32.138775, 31.821843,
                                                31.533121, 31.223797, 30.9226, 30.649842, 30.38037, 30.125075, 29.874417,
                                                29.637182, 29.410163, 29.184399, 28.967887, 28.748432, 28.537412,
                                                28.336286, 28.142074, 27.952112, 27.763106, 27.554904, 27.355905,
                                                27.163331, 26.961636, 26.755006, 26.539136, 26.322431, 26.095423,
                                                25.867645, 25.637042, 25.402191, 25.155426, 24.900057, 24.639498,
                                                24.353394, 24.069377, 23.78243, 23.489127, 23.185162, 22.876163, 22.581776,
                                                22.300379, 22.014197, 21.738425, 21.477526, 21.218674, 20.982489,
                                                20.741723, 20.509995, 20.28618, 20.063595, 19.845714, 19.625256, 19.407225,
                                                19.188555, 18.965879, 18.715538, 18.485834, 18.247306, 17.989033,
                                                17.730006,17.47415, 17.203123, 16.93376, 16.6481, 16.36511, 16.081017,
						15.78858, 15.510825, 15.234954, 14.960043, 14.689829, 14.43013, 14.181921,
                                                13.937926, 13.699244, 13.469614, 13.258739, 13.064613, 12.895495,
                                                12.757626, 12.645281, 12.563377, 12.518963};


  G4OpticalSurface*  ReflectorSkinSurface = new G4OpticalSurface("ReflectorSurface");
  ReflectorSkinSurface->SetType(dielectric_metal);
  ReflectorSkinSurface->SetModel(unified);
  ReflectorSkinSurface->SetFinish(polished);


  G4MaterialPropertiesTable *ref = new G4MaterialPropertiesTable();
  ref->AddProperty("REFLECTIVITY", ENERGY_ref, REFLECTIVITY_ref, NUMENTRIES_ref);
  ReflectorSkinSurface->SetMaterialPropertiesTable(ref);
  Aluminum->SetMaterialPropertiesTable(ref);



  const G4int numEntries = 2;
  const G4int NUMENTRIES_water = 60;

  G4double photonEnergy[numEntries] = {1.*eV, 10.*eV};
  G4double absorption[numEntries] = {absorptionCoeff, absorptionCoeff};
  G4double rindexWorld[numEntries] = {1.0, 1.0};

G4double RINDEX_air[NUMENTRIES_water] =
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0,
    1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

G4double BLACKABS_blacksheet[NUMENTRIES_water] =
  { 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm,
    1.0e9*cm,
    1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm,
    1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm,
    1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm,
    1.0e9*cm, 1.0e9*cm,
    1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e8*cm,
    1.0e9*cm,
    1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e8*cm, 1.0e9*cm,
    1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e8*cm, 1.0e9*cm,
    1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e9*cm, 1.0e8*cm, 1.0e9*cm,
    1.0e9*cm, 1.0e9*cm};


 G4double RAYLEIGH_air[NUMENTRIES_water] =
   { 0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,
     0.001*m,0.001*m,0.001*m,0.001*m,0.001*m,0.001*m};
 
 G4double ENERGY_water[NUMENTRIES_water] =
   { 1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV,
     1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV,
     1.74647*eV, 1.77142*eV, 1.7971*eV, 1.82352*eV,
     1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV,
     1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
     2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV,
     2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV,
     2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV,
     2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV,
     2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
     3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV,
     3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV,
     3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV,
     4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV,
     5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV };

 G4double ENERGY_glass[45]={4.95937*eV, 4.76862*eV, 4.59201*eV, 4.42801*eV, 4.27532*eV,
			    4.13281*eV, 3.99949*eV, 3.87451*eV, 3.7571*eV, 3.64659*eV,
			    3.54241*eV, 3.44401*eV, 3.35092*eV, 3.26274*eV, 3.17908*eV,
			    3.0996*eV, 3.024*eV, 2.952*eV, 2.88335*eV, 2.81782*eV,
			    2.7552*eV, 2.69531*eV, 2.63796*eV, 2.583*eV, 2.53029*eV,
			    2.47968*eV, 2.43106*eV, 2.38431*eV, 2.33932*eV, 2.296*eV,
			    2.25426*eV, 2.214*eV, 2.17516*eV, 2.13766*eV, 2.10143*eV,
			    2.0664*eV, 2.03253*eV, 1.99975*eV, 1.968*eV, 1.93725*eV, 
			    1.90745*eV, 1.87855*eV, 1.85051*eV, 1.8233*eV,  1.79687*eV };


G4double RINDEX1[NUMENTRIES_water] =
  {1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014,
   1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
   1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
   1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
   1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
   1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
   1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
   1.38362, 1.39074, 1.39956, 1.41075, 1.42535};

 G4double ABWFF = 1.30;

G4double ABSORPTION_water[NUMENTRIES_water] =
  {
    16.1419*cm*ABWFF,  18.278*cm*ABWFF, 21.0657*cm*ABWFF, 24.8568*cm*ABWFF, 30.3117*cm*ABWFF,
    38.8341*cm*ABWFF, 54.0231*cm*ABWFF, 81.2306*cm*ABWFF, 120.909*cm*ABWFF, 160.238*cm*ABWFF,
    193.771*cm*ABWFF, 215.017*cm*ABWFF, 227.747*cm*ABWFF,  243.85*cm*ABWFF, 294.036*cm*ABWFF,
    321.647*cm*ABWFF,  342.81*cm*ABWFF, 362.827*cm*ABWFF, 378.041*cm*ABWFF, 449.378*cm*ABWFF,
    739.434*cm*ABWFF, 1114.23*cm*ABWFF, 1435.56*cm*ABWFF, 1611.06*cm*ABWFF, 1764.18*cm*ABWFF,
    2100.95*cm*ABWFF,  2292.9*cm*ABWFF, 2431.33*cm*ABWFF,  3053.6*cm*ABWFF, 4838.23*cm*ABWFF,
    6539.65*cm*ABWFF, 7682.63*cm*ABWFF, 9137.28*cm*ABWFF, 12220.9*cm*ABWFF, 15270.7*cm*ABWFF,
    19051.5*cm*ABWFF, 23671.3*cm*ABWFF, 29191.1*cm*ABWFF, 35567.9*cm*ABWFF,   42583*cm*ABWFF,
    49779.6*cm*ABWFF, 56465.3*cm*ABWFF,   61830*cm*ABWFF, 65174.6*cm*ABWFF, 66143.7*cm*ABWFF,
    64820*cm*ABWFF,   61635*cm*ABWFF, 57176.2*cm*ABWFF, 52012.1*cm*ABWFF, 46595.7*cm*ABWFF,
    41242.1*cm*ABWFF, 36146.3*cm*ABWFF, 31415.4*cm*ABWFF, 27097.8*cm*ABWFF, 23205.7*cm*ABWFF,
    19730.3*cm*ABWFF, 16651.6*cm*ABWFF, 13943.6*cm*ABWFF, 11578.1*cm*ABWFF, 9526.13*cm*ABWFF
  };

 G4double RAYFF = 0.75;

 G4double RAYLEIGH_water[NUMENTRIES_water] = {
   386929*cm*RAYFF,  366249*cm*RAYFF,  346398*cm*RAYFF,  327355*cm*RAYFF,  309097*cm*RAYFF,
   291603*cm*RAYFF,  274853*cm*RAYFF,  258825*cm*RAYFF,  243500*cm*RAYFF,  228856*cm*RAYFF,
   214873*cm*RAYFF,  201533*cm*RAYFF,  188816*cm*RAYFF,  176702*cm*RAYFF,  165173*cm*RAYFF,
   154210*cm*RAYFF,  143795*cm*RAYFF,  133910*cm*RAYFF,  124537*cm*RAYFF,  115659*cm*RAYFF,
   107258*cm*RAYFF, 99318.2*cm*RAYFF, 91822.2*cm*RAYFF,   84754*cm*RAYFF, 78097.3*cm*RAYFF,
   71836.5*cm*RAYFF,   65956*cm*RAYFF, 60440.6*cm*RAYFF, 55275.4*cm*RAYFF, 50445.6*cm*RAYFF,
   45937*cm*RAYFF, 41735.2*cm*RAYFF, 37826.6*cm*RAYFF, 34197.6*cm*RAYFF, 30834.9*cm*RAYFF,
   27725.4*cm*RAYFF, 24856.6*cm*RAYFF, 22215.9*cm*RAYFF, 19791.3*cm*RAYFF, 17570.9*cm*RAYFF,
   15543*cm*RAYFF, 13696.6*cm*RAYFF, 12020.5*cm*RAYFF, 10504.1*cm*RAYFF, 9137.15*cm*RAYFF,
   7909.45*cm*RAYFF,  6811.3*cm*RAYFF, 5833.25*cm*RAYFF,  4966.2*cm*RAYFF, 4201.36*cm*RAYFF,
   3530.28*cm*RAYFF, 2944.84*cm*RAYFF, 2437.28*cm*RAYFF, 2000.18*cm*RAYFF,  1626.5*cm*RAYFF,
   1309.55*cm*RAYFF, 1043.03*cm*RAYFF, 821.016*cm*RAYFF,  637.97*cm*RAYFF, 488.754*cm*RAYFF
 };

 
 G4double RINDEX_glass[NUMENTRIES_water] =
   { 1.5110, 1.5112, 1.5114, 1.5116, 1.5118,
     1.5121, 1.5123, 1.5126, 1.5128, 1.5131,
     1.5133, 1.5136, 1.5139, 1.5142, 1.5145,
     1.5148, 1.5152, 1.5155, 1.5159, 1.5163,
     1.5167, 1.5171, 1.5176, 1.5180, 1.5185,
     1.5190, 1.5196, 1.5202, 1.5208, 1.5214,
     1.5221, 1.5228, 1.5236, 1.5244, 1.5253,
     1.5263, 1.5273, 1.5284, 1.5296, 1.5309,
     1.5323, 1.5338, 1.5354, 1.5372, 1.5392,
     1.5414, 1.5438, 1.5465, 1.5495, 1.5528,
     1.5566, 1.5608, 1.5657, 1.5713, 1.5777,
     1.5853, 1.5941, 1.6047, 1.6173, 1.6328 };

 G4double RINDEX_glass2[45]={1.578110, 1.571475, 1.565786, 1.560859, 1.556556,
			     1.552770, 1.549417, 1.546429, 1.543752, 1.541344,
			     1.539166, 1.537190, 1.535390, 1.533745, 1.532236, 
			     1.530849, 1.529569, 1.528385, 1.527288, 1.526269,
			     1.525320, 1.524433, 1.523605, 1.522829, 1.522100,
			     1.521414, 1.520769, 1.520160, 1.519584, 1.519039,
			     1.518522, 1.518032, 1.517566, 1.517122, 1.516699,
			     1.516295, 1.515909, 1.515539, 1.515186, 1.514846,
			     1.514520, 1.514207, 1.513905, 1.513615, 1.513335};




  G4double ABSORPTION_glass[NUMENTRIES_water]=
    { 1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm, 1.0e9*cm,
      1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,1.0e9*cm,
      1.0e9*cm, 1.0e9*cm };

  G4double ABSORPTION_glass2[45]={0.000574415*m, 0.000854808*m, 0.00134354*m, 0.00226156*m, 0.00414292*m,
				  0.00834524*m, 0.0180342*m, 0.0382308*m, 0.0785805*m, 0.143885*m,
				  0.299827*m, 0.629713*m, 1.07439*m, 1.45803*m, 2.26023*m, 
				  3.11245*m, 3.75419*m, 3.55892*m, 3.2194*m, 3.11393*m, 
				  3.30197*m, 3.55878*m, 3.6993*m, 3.77316*m, 3.88867*m, 
				  4.15414*m, 4.63536*m, 5.22806*m, 5.78547*m, 6.16079*m, 
				  6.21063*m, 5.92339*m, 5.45867*m, 4.98751*m, 4.64493*m, 
				  4.42188*m, 4.27329*m, 4.15408*m, 4.03614*m, 3.95912*m,
				  3.97959*m, 4.15417*m, 4.51931*m, 5.031*m, 5.31356*m};



  
  //  G4MaterialPropertiesTable *myMPT = new G4MaterialPropertiesTable();
  //  myMPT->AddProperty("RINDEX", ENERGY_water, RINDEX_air, NUMENTRIES_water);
  //  myMPT->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water); 
  //  myMPT->AddProperty("RAYLEIGH",ENERGY_water, RAYLEIGH_air, NUMENTRIES_water);

  //  Air->SetMaterialPropertiesTable(myMPT);

  G4MaterialPropertiesTable *myMPT2 = new G4MaterialPropertiesTable();
  myMPT2->AddProperty("RINDEX", ENERGY_water, RINDEX_air, NUMENTRIES_water);
  //myMPT2->AddProperty("ABSLENGTH", ENERGY_water, BLACKABS_blacksheet, NUMENTRIES_water);
  //  myMPT2->AddProperty("RAYLEIGH",ENERGY_water, RAYLEIGH_air, NUMENTRIES_water);

  Air1->SetMaterialPropertiesTable(myMPT2);


  G4MaterialPropertiesTable* absorberProperties = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable* mptWorld = new G4MaterialPropertiesTable();

  absorberProperties->AddProperty("ABSLENGTH", photonEnergy, absorption, numEntries);
  mptWorld->AddProperty("RINDEX", photonEnergy, rindexWorld, numEntries);
  mptWorld->AddProperty("ABSLENGTH", photonEnergy, absorption, numEntries);

  absorberMaterial->SetMaterialPropertiesTable(absorberProperties);
  Air->SetMaterialPropertiesTable(mptWorld);


  G4MaterialPropertiesTable *myMPT1 = new G4MaterialPropertiesTable();
  myMPT1->AddProperty("RINDEX", ENERGY_water, RINDEX1, NUMENTRIES_water);
  myMPT1->AddProperty("ABSLENGTH",ENERGY_water, ABSORPTION_water, NUMENTRIES_water);
  myMPT1->AddProperty("RAYLEIGH",ENERGY_water,RAYLEIGH_water,NUMENTRIES_water);

  Water->SetMaterialPropertiesTable(myMPT1);

  /////////PMT Glass Material///////////////  
  density = 2.20*g/cm3;
  G4Material* SiO2 = new G4Material("SiO2", density, 2);
  SiO2->AddElement(nist->FindOrBuildElement("Si"), 1);
  SiO2->AddElement(nist->FindOrBuildElement("O"), 2);
  
  a = 10.81*g/mole;
  G4Element* B = new G4Element("Boron", "B", 5, a);

  density= 2.46*g/cm3;
  G4Material* B2O3 = new G4Material("B2O3", density, 2);
  B2O3->AddElement(nist->FindOrBuildElement("B"), 2);
  B2O3->AddElement(nist->FindOrBuildElement("O"), 3);

  a = 22.99*g/mole;
  G4Element* Na = new G4Element("Sodium", "Na", 11, a);

  density= 2.47*g/cm3;
  G4Material* Na2O = new G4Material("Na2O", density, 2);
  Na2O->AddElement(nist->FindOrBuildElement("Na"), 2);
  Na2O->AddElement(nist->FindOrBuildElement("O"), 1);

  density= 4.00*g/cm3;
  G4Material* Al2O3 = new G4Material("Al2O3", density, 2);
  Al2O3->AddElement(nist->FindOrBuildElement("Al"), 2);
  Al2O3->AddElement(nist->FindOrBuildElement("O"), 3);

  density = 2.23*g/cm3;
  G4Material* PMTGlass = new G4Material("PMTGlass", density, 4);
  PMTGlass->AddMaterial(SiO2, 80.6*perCent);
  PMTGlass->AddMaterial(B2O3, 13.0*perCent);
  PMTGlass->AddMaterial(Na2O, 4.0*perCent);
  PMTGlass->AddMaterial(Al2O3, 2.4*perCent);

  G4MaterialPropertiesTable *myMPT5 = new G4MaterialPropertiesTable();
  std::reverse(std::begin(ENERGY_glass), std::end(ENERGY_glass));
  std::reverse(std::begin(RINDEX_glass2), std::end(RINDEX_glass2));
  std::reverse(std::begin(ABSORPTION_glass2), std::end(ABSORPTION_glass2));
  myMPT5->AddProperty("RINDEX", ENERGY_glass, RINDEX_glass2, 45);
  myMPT5->AddProperty("ABSLENGTH",ENERGY_glass, ABSORPTION_glass2, 45);
  PMTGlass->SetMaterialPropertiesTable(myMPT5);

  /////////////////////////////////////////
  G4double pmtGlassThickness = 2.0*mm;
  G4double pmtGlassRadius = 53.0*mm;
  G4double pmtCylRadius = 40.15*mm;
  G4double pmtTubeRadius = 26.1*mm;
  G4double pmtCylHeight = 18.0*mm;
  G4double pmtConeHeight = 20.0*mm;


  G4Box *solidWorld = new G4Box("solidWorld", 0.5*m, 0.5*m, 0.5*m);
  
  G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, 
						    //Air1, 
						    Water,
						    "logicWorld");
  
  G4VPhysicalVolume *physWorld = new G4PVPlacement(0, 
						   G4ThreeVector(0.,0.,0.), 
						   logicWorld, 
						   "physWorld", 
						   0, 
						   false, 
						   0, 
						   true);
  
  G4Cons *SolidCone = new G4Cons("pmtCone", 
				 pmtTubeRadius-pmtGlassThickness, 
				 pmtTubeRadius, 
				 pmtCylRadius-pmtGlassThickness, 
				 pmtCylRadius, 
				 pmtConeHeight/2.*mm, 
				 0.*deg, 360.*deg);  
  
  G4Tubs *tube = new G4Tubs("tube", 
			    pmtCylRadius-pmtGlassThickness, 
			    pmtCylRadius, 
			    pmtCylHeight/2.*mm, 
			    0.*deg, 360.*deg);
  /////////////////////Check contact////////////////////
  /*
  G4SubtractionSolid* intersection = new G4SubtractionSolid("intersection", tube, SolidCone);
  G4bool touching = (intersection->GetCubicVolume() > 0.0);


  if(touching)
    {
      G4cout<<"The solids are touching each other with no space"<<G4endl;
    }
  else
    {
      G4cout<<"There is a space between the solids"<<G4endl;
    }
*/
  //////////////////////////////////////////////
  
  G4UnionSolid *combineSolid1 = new G4UnionSolid("tubeplusreflection", 
						 SolidCone, 
						 tube, 
						 0, 
						 G4ThreeVector(0.,0.,19.0*mm));
  
  G4LogicalVolume *logicCombine1 = new G4LogicalVolume(combineSolid1,
                                                      PMTGlass,
                                                      "logicCombine1");

  G4VPhysicalVolume *glassTube = new G4PVPlacement(0,
                                                   G4ThreeVector(0.,0.,0.),
                                                   logicCombine1,
                                                   "glassTube",
                                                   logicWorld,
                                                   false,
                                                   0,
                                                   true);
  G4Box *solidCutOff = new G4Box("cutOffTubs",                                                                            
                                 pmtGlassRadius+1.*cm,                                                                     
                                 pmtGlassRadius+1.*cm,                                                                    
                                 34.597*mm);

  G4Sphere *pmtGlassSolid = new G4Sphere("pmtglass", 
					 pmtGlassRadius-pmtGlassThickness, 
					 pmtGlassRadius, 
					 0.0*deg, 360.0*deg, 
					 0.0, 90*deg);  
  
  G4SubtractionSolid *pmtBulb = new G4SubtractionSolid("pmtInnerglass",                                       
						       pmtGlassSolid,                                   
						       solidCutOff); 

  //  G4Transform3D transform(G4Translate3D(0.0, 0.0, -18.097*mm));
  G4Transform3D transform(G4Translate3D(0.0, 0.0, -17.702*mm));
  /*
  G4UnionSolid *combineSolid2 = new G4UnionSolid("glassplustubeplusreflection", 
						 combineSolid1, 
						 pmtGlass, 
						 transform);
  */
  
  pmtGlassLogic = new G4LogicalVolume(pmtBulb, 
				      PMTGlass, 
				      "pmtGlassLogic");
  
  G4VPhysicalVolume *pmtSphere = new G4PVPlacement(0, 
						   //G4ThreeVector(0.,0.,-18.097*mm), 
						   G4ThreeVector(0.,0.,-6.597*mm), 
						   pmtGlassLogic, 
						   "pmtSphere", 
						   logicWorld, 
						   false, 
						   0, 
						   true);
  fScoringPMTVolume = pmtGlassLogic;

  ///////////////////PMT INNER/////////////////////////////////
  
  G4Cons *innerSolidCone = new G4Cons("innerPmtCone",
                                 0.,
                                 pmtTubeRadius-pmtGlassThickness-0.1*mm,
                                 0.,
                                 pmtCylRadius-pmtGlassThickness-0.1*mm,
				 pmtConeHeight/2.*mm,
                                 0.*deg, 360.*deg);
  /*
  G4LogicalVolume *innerLogicCone = new G4LogicalVolume(innerSolidCone, Air1, "innerLogicCone");
  G4VPhysicalVolume *placeInnerCone = new G4PVPlacement(0,
						    G4ThreeVector(0.,0.,0.0*mm),
						    innerLogicCone,
						    "placeInnerCone",
						    logicWorld,
						    false,
						    0,
						    true);
   */
  G4Tubs *innerTube = new G4Tubs("innerTube",
				 0.,
				 pmtCylRadius-pmtGlassThickness-0.1*mm,
				 //8.303*mm,
				 9.0*mm,
				 0.*deg, 360.*deg);


  G4UnionSolid *innerCombineSolid = new G4UnionSolid("innerCombineSolid",
                                                 innerSolidCone,
                                                 innerTube,
                                                 0,
                                                 G4ThreeVector(0.,0.,19.0*mm));

  G4LogicalVolume *innerLogicTube = new G4LogicalVolume(innerCombineSolid, 
							//Vacuum,
							Air1, 
							"innerLogicTube");
  
  G4VPhysicalVolume *placeInnerTube = new G4PVPlacement(0,
							G4ThreeVector(0.,0.,0.0*mm),
							innerLogicTube,
							"placeInnerCone",
							logicWorld,
							false,
							0,
							true);

  
  
  /*
  ////////////////////Checking OverLap///////////////////////////////////
  G4SubtractionSolid* intersection = new G4SubtractionSolid("intersection", pmtGlassSolid, tube);
  G4bool touching = (intersection->GetCubicVolume() > 0.0);


  if(touching)
    {
      G4cout<<"The solids are touching each other with no space"<<G4endl;
    }
  else
    {
      G4cout<<"There is a space between the solids"<<G4endl;
    }
  ////////////////////////////////////////////////////////////////////////
  
  G4double PMTOffset = 34.597*mm;

  G4Box *solidCutOff = new G4Box("cutOffTubs",
				 pmtGlassRadius+1.*cm,
				 pmtGlassRadius+1.*cm,
				 PMTOffset);

  G4Sphere *tmPmtInnerGlassSolid = new G4Sphere("tmPmtInnerGlassSolid",
						0.0*mm,pmtGlassRadius,
						0.0*deg, 360.0*deg,
						0.0, 90.*deg);

  G4SubtractionSolid *pmtInnerGlassSolid = new G4SubtractionSolid("pmtInnerglass",
								  tmPmtInnerGlassSolid,
								  solidCutOff);
  
  */
  G4Sphere *pmtInnerGlassSolid = new G4Sphere("pmtInnerglassSolid",
					      0.*mm,
					      //pmtCylRadius-0.3*mm,
					      pmtGlassRadius-pmtGlassThickness,
					      0.0*deg, 360.0*deg,
					      0.0, 90*deg);
  G4Box *boxCutOff = new G4Box("cutOffTubs",                                                                            
			       pmtGlassRadius+1.*cm,                                                    
			       pmtGlassRadius+1.*cm,                                             
			       34.597);
			       //33.9*mm);   

  G4SubtractionSolid *pmtInnerGlass = new G4SubtractionSolid("pmtInnerglass",     
								  pmtInnerGlassSolid,
                                                                  boxCutOff); 

  pmtInnerGlassLogic = new G4LogicalVolume(pmtInnerGlass,
					   Air1,
					   //Vacuum,
					   "pmtInnerGlassLogic");
  
  G4VPhysicalVolume *pmtInnerSphere = new G4PVPlacement(0,
							G4ThreeVector(0.,0.,-6.597*mm),
							pmtInnerGlassLogic,
							"pmtInnerSphere",
							logicWorld,
							false,
							0,
							true);
  
  //  fScoringInnerPMTVolume = pmtInnerGlassLogic;
////////////////////Photon Absorber//////////////////////
//  G4cout<<"Successfully entered the photon adsorber\n"<<G4endl;

  
  G4Cons* solidAbsorber = new G4Cons("solidAbsorber", 
				     0.0, 
				     pmtTubeRadius, 
				     0.0, 
				     pmtTubeRadius, 
				     absorberheight, 
				     0.0, 360.0*deg);
  



  //  G4ThreeVector absorberPosition(0.,0.,-5.5*mm);
  G4ThreeVector absorberPosition(0.,0.,-10.5*mm);
  
  logicabsorber = new G4LogicalVolume(solidAbsorber, 
				      absorberMaterial, 
				      "logicabsorber");
  
  G4VPhysicalVolume *absorber = new G4PVPlacement(0, 
						  absorberPosition, 
						  logicabsorber, 
						  "absorber", 
						  logicWorld, 
						  false, 
						  0, 
						  true);

  G4VisAttributes *visAttributes = new G4VisAttributes();
  visAttributes->SetColor(G4Color::Gray());
  visAttributes->SetVisibility(true);
  visAttributes->SetForceSolid(true);
  logicabsorber->SetVisAttributes(visAttributes);

  fScoringVolume = logicabsorber;

  

  //  G4cout<<"Successfully Exited the photon adsorber\n"<<G4endl;

  //////////////////Optical photocathode surface and its properties////////////////////////
  const G4int NUM = 2;
  
  G4OpticalSurface* OpGlassCathodeSurface =  new G4OpticalSurface("GlassCathodeSurface");
  OpGlassCathodeSurface->SetType(dielectric_dielectric);
  OpGlassCathodeSurface->SetModel(unified);
  OpGlassCathodeSurface->SetFinish(polished);

  G4double RINDEX_cathode[NUM] = { 1.0, 1.0 };
  G4double PP[NUM] = { 1.4E-9*GeV,6.2E-9*GeV};
  G4double RGCFF = 0.32;
  //  RGCFF = WCSimTuningParams->GetRgcff();
  G4double REFLECTIVITY_glasscath[NUM] = { 1.0*RGCFF, 1.0*RGCFF };
  G4double EFFICIENCY_glasscath[NUM] = { 0.0, 0.0 };
  G4int COATEDFRUSTRATEDTRANSMISSION_glasscath = 1;

  const G4int NUMSK = 6;
  G4double ENERGY_COATED_SK[NUMSK] =
    {
      1.000*eV, 2.786*eV, 3.061*eV, 3.306*eV, 3.679*eV, 9.000*eV
    };
  // Real refractive index of photocathode film                                                                             
  G4double COATEDRINDEX_glasscath_SK[NUMSK] =
    { 3.4, 3.4, 3.1, 2.8, 2.4, 2.4 };
  // Imaginary refractive index of photocathode film                                                                        
  G4double COATEDRINDEXIM_glasscath_SK[NUMSK] =
    { 1.7, 1.7, 1.6, 1.5, 1.4, 1.4 };
  // Thickness of photocathode film                                                                                         
  G4double COATEDTHICKNESS_glasscath_SK = 11.5*nm;
  
  const G4int NUMWAV = 23;
  G4double ENERGY_COATED_WAV[NUMWAV] =
    {
      1.000*eV, 1.823*eV, 1.864*eV, 1.907*eV, 1.953*eV,
      2.000*eV, 2.049*eV, 2.101*eV, 2.156*eV, 2.214*eV,
      2.275*eV, 2.339*eV, 2.407*eV, 2.480*eV, 2.556*eV,
      2.638*eV, 2.725*eV, 2.818*eV, 2.917*eV, 3.024*eV,
      3.139*eV, 3.263*eV, 9.000*eV
    };
  G4double COATEDRINDEX_glasscath_KCsCb[NUMWAV] =
    {
      2.96, 2.96, 2.95, 2.95, 2.95,
      2.96, 2.98, 3.01, 3.06, 3.12,
       3.20, 3.26, 3.09, 3.00, 3.00,
      3.00, 2.87, 2.70, 2.61, 2.38,
      2.18, 1.92, 1.92
    };
  G4double COATEDRINDEXIM_glasscath_KCsCb[NUMWAV] =
    {
      0.33, 0.33, 0.34, 0.34, 0.35,
      0.37, 0.38, 0.42, 0.46, 0.53,
      0.63, 0.86, 1.05, 1.06, 1.11,
      1.34, 1.44, 1.50, 1.53, 1.71,
      1.69, 1.69, 1.69
    };
  G4double COATEDTHICKNESS_glasscath_KCsCb = 20.0*nm;
  //  G4double COATEDTHICKNESS_glasscath_KCsCb = 10.*nm;
  
  G4double COATEDRINDEX_glasscath_RbCsCb[NUMWAV] =
    {
      3.13, 3.13, 3.14, 3.14, 3.15,
      3.18, 3.22, 3.28, 3.39, 3.32,
      3.23, 3.21, 3.22, 3.16, 2.99,
      2.81, 2.63, 2.50, 2.40, 2.30,
      2.22, 2.07, 2.07
    };
  G4double COATEDRINDEXIM_glasscath_RbCsCb[NUMWAV] =
    {
       0.35, 0.35, 0.37, 0.37, 0.38,
       0.40, 0.43, 0.46, 0.59, 0.76,
       0.86, 0.90, 1.04, 1.21, 1.37,
       1.41, 1.40, 1.35, 1.27, 1.21,
       1.17, 1.22, 1.22
     };

  G4double COATEDTHICKNESS_glasscath_RbCsCb = 23.4*nm;
  
  //Glass to Cathode surface inside PMTs
  G4MaterialPropertiesTable *myST2 = new G4MaterialPropertiesTable();
  myST2->AddProperty("RINDEX", PP, RINDEX_cathode, NUM);
  myST2->AddProperty("REFLECTIVITY", PP, REFLECTIVITY_glasscath, NUM);
  myST2->AddProperty("EFFICIENCY", PP, EFFICIENCY_glasscath, NUM);

  // In order to use new photocathode physics
  G4int pmtsurftype = WCSimTuningParams->GetPMTSurfType(); // Choose one of the two models, see WCSimOpBoundaryProcess for model details                                                                                                                
  G4cout<<"PMT SURFACE == "<<pmtsurftype<<G4endl;
  
  if (pmtsurftype==1)
    {
      OpGlassCathodeSurface->SetType(G4SurfaceType(101));
    }
  else if (pmtsurftype==2)
    {
      OpGlassCathodeSurface->SetType(G4SurfaceType(102));
    }
  else if (pmtsurftype!=0)
    {
      printf("Invalid PMT photocathode surface optical model choice: %i, use the default dielectric model\n",pmtsurftype);
      pmtsurftype = 0;
    }
  if (pmtsurftype!=0)
    {
      G4int cathodepara = WCSimTuningParams->GetCathodePara(); // Choose predefined set of optical parameters           
      G4cout<<"CATHODE PARAMETER == "<<cathodepara<<G4endl;
      if (cathodepara==0)
	{
	  printf("Use SK cathode optical parameters\n");
	  myST2->AddProperty("COATEDRINDEX", ENERGY_COATED_SK, COATEDRINDEX_glasscath_SK, NUMSK);
	  myST2->AddProperty("COATEDRINDEXIM", ENERGY_COATED_SK, COATEDRINDEXIM_glasscath_SK, NUMSK);
	  myST2->AddConstProperty("COATEDTHICKNESS", COATEDTHICKNESS_glasscath_SK);
	  myST2->AddConstProperty("COATEDFRUSTRATEDTRANSMISSION", COATEDFRUSTRATEDTRANSMISSION_glasscath);
	}
      else if (cathodepara==1)
	{
	  printf("Use KCsCb cathode optical parameters\n");
	  myST2->AddProperty("COATEDRINDEX", ENERGY_COATED_WAV, COATEDRINDEX_glasscath_KCsCb, NUMWAV);
	  myST2->AddProperty("COATEDRINDEXIM", ENERGY_COATED_WAV, COATEDRINDEXIM_glasscath_KCsCb, NUMWAV);
	  myST2->AddConstProperty("COATEDTHICKNESS", COATEDTHICKNESS_glasscath_KCsCb);
	  myST2->AddConstProperty("COATEDFRUSTRATEDTRANSMISSION", COATEDFRUSTRATEDTRANSMISSION_glasscath);
	}
      else if (cathodepara==2)
	{
	  printf("Use RbCsCb cathode optical parameters\n");
	  myST2->AddProperty("COATEDRINDEX", ENERGY_COATED_WAV, COATEDRINDEX_glasscath_RbCsCb, NUMWAV);
	  myST2->AddProperty("COATEDRINDEXIM", ENERGY_COATED_WAV, COATEDRINDEXIM_glasscath_RbCsCb, NUMWAV);
	  myST2->AddConstProperty("COATEDTHICKNESS", COATEDTHICKNESS_glasscath_RbCsCb);
	  myST2->AddConstProperty("COATEDFRUSTRATEDTRANSMISSION", COATEDFRUSTRATEDTRANSMISSION_glasscath);
	}
      else
	{
	  printf("Invalid PMT photocathode surface parameters choice: %i, use the default SK model\n",pmtsurftype);
	  myST2->AddProperty("COATEDRINDEX", ENERGY_COATED_SK, COATEDRINDEX_glasscath_SK, NUMSK);
	  myST2->AddProperty("COATEDRINDEXIM", ENERGY_COATED_SK, COATEDRINDEXIM_glasscath_SK, NUMSK);
	  myST2->AddConstProperty("COATEDTHICKNESS", COATEDTHICKNESS_glasscath_SK);
	  myST2->AddConstProperty("COATEDFRUSTRATEDTRANSMISSION", COATEDFRUSTRATEDTRANSMISSION_glasscath);
	}
    }
  

  OpGlassCathodeSurface->SetMaterialPropertiesTable(myST2);

  G4LogicalBorderSurface* GlassCathodeSurface = new G4LogicalBorderSurface("GlassCathodeSurface", pmtSphere, pmtInnerSphere, OpGlassCathodeSurface);

  G4cout << "Scoring volume:  " << fScoringVolume->GetName() << G4endl;  
  
  //Adding a reflector material inside the PMT

  G4Cons *solidConeTube = new G4Cons("ConeTube",
				 pmtTubeRadius-pmtGlassThickness-0.1*mm,
				 pmtTubeRadius-pmtGlassThickness,
				 pmtCylRadius-pmtGlassThickness-0.1*mm,
				 pmtCylRadius-pmtGlassThickness-0.09*mm,
				 10.*mm,
				 0.*deg, 360.*deg);

  
  G4LogicalVolume *logicLayer = new G4LogicalVolume(solidConeTube, 
						    Aluminum, 
						    "logicLayer");

  G4VisAttributes *layerAttributes = new G4VisAttributes();
  layerAttributes->SetColor(0.85, 0.85, 0.85, 1.0);
  layerAttributes->SetVisibility(true);
  layerAttributes->SetForceSolid(true);
  logicLayer->SetVisAttributes(layerAttributes);

  G4LogicalSkinSurface* ReflectorLogSkinSurface = new G4LogicalSkinSurface("ReflectorLogSkinSurface", logicLayer, ReflectorSkinSurface);
 
  G4VPhysicalVolume *placeLayer = new G4PVPlacement(0,
                                                   G4ThreeVector(0.,0.,0.),
                                                   logicLayer,
                                                   "placeLayer",
                                                   logicWorld,
                                                   false,
                                                   0,
                                                   true);
  

  
  //delete steppingAction;
  return logicWorld;
}

G4VPhysicalVolume *myDetectorConstruction::Construct()
{
  return nullptr;

}
