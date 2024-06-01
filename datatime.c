void datatime()
{
    TChain *tIn = new TChain("TofHitsTree");
    tIn->Add("/home/lecamvi/Documents/TOF_thesis/output/run2134_outputTree.root");

    std::vector<Double_t> *SignalPosition = nullptr;
    std::vector<Double_t> *SignalPlane = nullptr;
    std::vector<Double_t> *SignalTime = nullptr;
    std::vector<Double_t> *SignalPositionY = nullptr;
    std::vector<Double_t> *SignalPositionZ = nullptr;

    tIn->SetBranchAddress("SignalPosition", &SignalPosition);
    tIn->SetBranchAddress("SignalPlane", &SignalPlane);
    tIn->SetBranchAddress("SignalTime", &SignalTime);
    tIn->SetBranchAddress("SignalPositionY", &SignalPositionY);
    tIn->SetBranchAddress("SignalPositionZ", &SignalPositionZ);

    bool hasYCoord = (SignalPositionY != nullptr);
    bool hasZCoord = (SignalPositionZ != nullptr);

    // Khởi tạo histogram
    TH1F *h_Top = new TH1F("h_Top", "Top Signal Position", 100, 0, 100);
    TH1F *h_Bot = new TH1F("h_Bot", "Bottom Signal Position", 100, 0, 100);
    TH1F *h_Up = new TH1F("h_Up", "Up Signal Position", 100, 0, 100);
    TH1F *h_Down = new TH1F("h_Down", "Down Signal Position", 100, 0, 100);

    Long64_t nEntries = tIn->GetEntries();

    // Khởi tạo mảng
    std::vector<Double_t> planeT(nEntries, 0.0);
    std::vector<Double_t> planeB(nEntries, 0.0);
    std::vector<Double_t> planeU(nEntries, 0.0);
    std::vector<Double_t> planeD(nEntries, 0.0);
    std::vector<Double_t> planeYT(hasYCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> planeYB(hasYCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> planeYU(hasYCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> planeYD(hasYCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> planeZT(hasZCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> planeZB(hasZCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> planeZU(hasZCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> planeZD(hasZCoord ? nEntries : 0, 0.0);
    std::vector<Double_t> timeT(nEntries, 0.0);
    std::vector<Double_t> timeB(nEntries, 0.0);
    std::vector<Double_t> timeU(nEntries, 0.0);
    std::vector<Double_t> timeD(nEntries, 0.0);

    for (Long64_t i = 0; i < nEntries; i++)
    {
        tIn->GetEntry(i);

        // Tìm tọa độ x, y, z và thời gian của SignalPlane top, bot, up, và down
        for (size_t j = 0; j < SignalPosition->size(); j++)
        {
            if ((*SignalPlane)[j] == 3)
            {
                planeT[i] = (*SignalPosition)[j];
                if (hasYCoord && hasZCoord)
                {
                    planeYT[i] = (*SignalPositionY)[j];
                    planeZT[i] = (*SignalPositionZ)[j];
                }
                timeT[i] = (*SignalTime)[j];
                h_Top->Fill(planeT[i]);
            }
            else if ((*SignalPlane)[j] == 2)
            {
                planeB[i] = (*SignalPosition)[j];
                if (hasYCoord && hasZCoord)
                {
                    planeYB[i] = (*SignalPositionY)[j];
                    planeZB[i] = (*SignalPositionZ)[j];
                }
                timeB[i] = (*SignalTime)[j];
                h_Bot->Fill(planeB[i]);
            }
            else if ((*SignalPlane)[j] == 4)
            {
                planeU[i] = (*SignalPosition)[j];
                if (hasYCoord && hasZCoord)
                {
                    planeYU[i] = (*SignalPositionY)[j];
                    planeZU[i] = (*SignalPositionZ)[j];
                }
                timeU[i] = (*SignalTime)[j];
                h_Up->Fill(planeU[i]);
            }
            else if ((*SignalPlane)[j] == 5)
            {
                planeD[i] = (*SignalPosition)[j];
                if (hasYCoord && hasZCoord)
                {
                    planeYD[i] = (*SignalPositionY)[j];
                    planeZD[i] = (*SignalPositionZ)[j];
                }
                timeD[i] = (*SignalTime)[j];
                h_Down->Fill(planeD[i]);
            }
        }
    }

    // Khai báo biến numEvents
    int numEvents = planeT.size();

    // Mở file để ghi dữ liệu
    std::ofstream outfile("coordinates.txt");

    // In tọa độ và thời gian của SignalPlane top
    outfile << "Top Plane Coordinates and Time:" << std::endl;
    for (int i = 0; i < numEvents; i++) {
        if (hasYCoord && hasZCoord) {
            outfile << "Event " << i << ": x = " << planeT[i] << ", y = " << planeYT[i] << ", z = " << planeZT[i] << ", t = " << timeT[i] << std::endl;
        } else {
            outfile << "Event " << i << ": x = " << planeT[i] << ", t = " << timeT[i] << std::endl;
        }
    }
    outfile << std::endl;

    // In tọa độ và thời gian của SignalPlane bot
    outfile << "Bottom Plane Coordinates and Time:" << std::endl;
    for (int i = 0; i < numEvents; i++) {
        if (hasYCoord && hasZCoord) {
            outfile << "Event " << i << ": x = " << planeB[i] << ", y = " << planeYB[i] << ", z = " << planeZB[i] << ", t = " << timeB[i] << std::endl;
        } else {
            outfile << "Event " << i << ": x = " << planeB[i] << ", t = " << timeB[i] << std::endl;
        }
    }
    outfile << std::endl;

    // In tọa độ và thời gian của SignalPlane up
    outfile << "Up Plane Coordinates and Time:" << std::endl;
for (int i = 0; i < numEvents; i++) {
if (hasYCoord && hasZCoord) {
outfile << "Event " << i << ": x = " << planeU[i] << ", y = " << planeYU[i] << ", z = " << planeZU[i] << ", t = " << timeU[i] << std::endl;
} else {
outfile << "Event " << i << ": x = " << planeU[i] << ", t = " << timeU[i] << std::endl;
}
}
outfile << std::endl;
// In tọa độ và thời gian của SignalPlane down
outfile << "Down Plane Coordinates and Time:" << std::endl;
for (int i = 0; i < numEvents; i++) {
    if (hasYCoord && hasZCoord) {
        outfile << "Event " << i << ": x = " << planeD[i] << ", y = " << planeYD[i] << ", z = " << planeZD[i] << ", t = " << timeD[i] << std::endl;
    } else {
        outfile << "Event " << i << ": x = " << planeD[i] << ", t = " << timeD[i] << std::endl;
    }
}

// Đóng file
outfile.close();
}