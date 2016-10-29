// This little test application demonstrates how to create and modify
// Gigasampler files with libgig 3.0.0.
//
// Date: 2006-04-29
//
// Compile with: 'g++ -lgig -o gigwritedemo gigwritedemo.cpp'

#include <libgig/gig.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  // four stupid little sample "waves"
  // (each having three sample points length, 16 bit depth, mono)
  int16_t sampleData1[] = { 1, 2, 3 };
  int16_t sampleData2[] = { 4, 5, 6 };
  int16_t sampleData3[] = { 7, 8, 9 };
  int16_t sampleData4[] = { 10, 11, 12 };
  try {
    // create an empty Gigasampler file
    gig::File file;
    // we give it an internal name, not mandatory though
    file.pInfo->Name = "Foo Gigasampler File";

    // create four samples
    gig::Sample* pSample1 = file.AddSample();
    gig::Sample* pSample2 = file.AddSample();
    gig::Sample* pSample3 = file.AddSample();
    gig::Sample* pSample4 = file.AddSample();
    // give those samples a name (not mandatory)
    pSample1->pInfo->Name = "Foo Sample 1";
    pSample2->pInfo->Name = "Foo Sample 2";
    pSample3->pInfo->Name = "Foo Sample 3";
    pSample4->pInfo->Name = "Foo Sample 4";
    // set meta informations for those samples
    pSample1->Channels = 1; // mono
    pSample1->BitDepth = 16; // 16 bits
    pSample1->FrameSize = 16/*bitdepth*// 8/*1 byte are 8 bits*/* 1/*mono*/;
    pSample1->SamplesPerSecond = 44100;
    pSample2->Channels = 1; // mono
    pSample2->BitDepth = 16; // 16 bits
    pSample2->FrameSize = 16 / 8 * 1;
    pSample2->SamplesPerSecond = 44100;
    pSample3->Channels = 1; // mono
    pSample3->BitDepth = 16; // 16 bits
    pSample3->FrameSize = 16 / 8 * 1;
    pSample3->SamplesPerSecond = 44100;
    pSample4->Channels = 1; // mono
    pSample4->BitDepth = 16; // 16 bits
    pSample4->FrameSize = 16 / 8 * 1;
    pSample4->SamplesPerSecond = 44100;
    // resize those samples to a length of three sample points
    // (again: _sample_points_ NOT bytes!) which is the length of our
    // ficticious samples from above. after the Save() call below we can
    // then directly write our sample data to disk by using the Write()
    // method, that is without having to load all the sample data into
    // RAM. for large instruments / .gig files this is definitely the way
    // to go
    pSample1->Resize(3);
    pSample2->Resize(3);
    pSample3->Resize(3);
    pSample4->Resize(3);

    // create four instruments
    gig::Instrument* pInstrument1 = file.AddInstrument();
    gig::Instrument* pInstrument2 = file.AddInstrument();
    gig::Instrument* pInstrument3 = file.AddInstrument();
    gig::Instrument* pInstrument4 = file.AddInstrument();
    // give them a name (not mandatory)
    pInstrument1->pInfo->Name = "Foo Instrument 1";
    pInstrument2->pInfo->Name = "Foo Instrument 2";
    pInstrument3->pInfo->Name = "Foo Instrument 3";
    pInstrument4->pInfo->Name = "Foo Instrument 4";

    // create one region for each instrument
    // in this example we do not add a dimension, so
    // every region will have exactly one DimensionRegion
    gig::Region* pRegion = pInstrument1->AddRegion();
    pRegion->SetKeyRange(40,41);
    pRegion->SetSample(pSample1);
    pRegion->pDimensionRegions[0]->pSample = pSample1;

    pRegion = pInstrument2->AddRegion();
    pRegion->SetSample(pSample2);
    pRegion->pDimensionRegions[0]->pSample = pSample2;

    pRegion = pInstrument3->AddRegion();
    pRegion->SetSample(pSample3);
    pRegion->pDimensionRegions[0]->pSample = pSample3;

    pRegion = pInstrument4->AddRegion();
    pRegion->SetSample(pSample4);
    pRegion->pDimensionRegions[0]->pSample = pSample4;

    // save file as of now
    file.Save("foo.gig");

    // now as the file exists physically and the 'samples' are already
    // of the correct size we can write the actual sample data by
    // directly writing to disk
    pSample1->Write(sampleData1, 3);
    pSample2->Write(sampleData2, 3);
    pSample3->Write(sampleData3, 3);
    pSample4->Write(sampleData4, 3);
  } catch (RIFF::Exception &e) {
    e.PrintMessage();
    return -1;
  }
  return 0;
}
