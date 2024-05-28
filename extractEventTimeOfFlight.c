#include <TFile.h>
#include <TTree.h>
#include <fstream>
#include <iostream>
#include <cmath>

// Định nghĩa cấu trúc để lưu trữ dữ liệu time-of-flight của sự kiện
struct Position {
    double x;
    double y;
    double z;
    int plane;
    double time;
    double position;
    int bar;
};

struct Event {
    Position start;
    Position end;
};

// Hàm tính tọa độ của một điểm trong mặt phẳng
bool getCoordinate(int Plane, int Bar, double coord, double &x, double &y, double &z) {
    // Các thông số từ file analysisPrime.pdf
    const double barLength = 350.0; // Độ dài của thanh (cm)
    const double barWidth = 6.0; // Chiều rộng của thanh (cm)
    const double barGap = 0.8; // Khoảng cách giữa các thanh (cm)
    const double planeGap = 77.0; // Khoảng cách giữa hai mặt phẳng (cm)

    // Tính tọa độ x, y, z của điểm trong mặt phẳng
    x = (Bar - 1) * (barWidth + barGap); // Tọa độ x
    y = coord; // Tọa độ y
    z = (Plane - 1) * planeGap; // Tọa độ z

    return true;
}

// Hàm tính tọa độ của hai đầu của thanh
bool getBarEnds(int Plane, int Bar, double &x1, double &y1, double &z1, double &x2, double &y2, double &z2) {
    // Các thông số từ file analysisPrime.pdf
    const double barLength = 350.0; // Độ dài của thanh (cm)
    const double barWidth = 6.0; // Chiều rộng của thanh (cm)
    const double barGap = 0.8; // Khoảng cách giữa các thanh (cm)
    const double planeGap = 77.0; // Khoảng cách giữa hai mặt phẳng (cm)

    // Tính tọa độ của hai đầu thanh
    double x, y, z;
    getCoordinate(Plane, Bar, 0.0, x, y, z);
    x1 = x;
    y1 = y;
    z1 = z;
    x2 = x;
    y2 = y + barLength;
    z2 = z;

    return true;
}

void extractEventTimeOfFlight() {
    // Khai báo các biến SignalTime và SignalPosition
    double SignalTime, SignalPosition;

    // Khai báo biến EventTimeOfFlight
    Event Event;

    // Mở tệp dữ liệu
    TFile* file = new TFile("/home/lecamvi/Documents/TOF_thesis/output/run16480_outputTree.root", "READ");

    // Lấy TTree từ tệp
    TTree* tree = (TTree*)file->Get("TofHitsTree");

    double startX, startY, startZ, endX, endY, endZ;
    int Plane, Bar;

    // Thiết lập địa chỉ nhánh cho các biến
    tree->Branch("start_x", &Event.start.x, "start_x/D");
    tree->Branch("start_y", &Event.start.y, "start_y/D");
    tree->Branch("start_z", &Event.start.z, "start_z/D");
    tree->Branch("start_plane", &Event.start.plane, "start_plane/I");
    tree->Branch("end_x", &Event.end.x, "end_x/D");
    tree->Branch("end_y", &Event.end.y, "end_y/D");
    tree->Branch("end_z", &Event.end.z, "end_z/D");
    tree->Branch("end_plane", &Event.end.plane, "end_plane/I");
    tree->Branch("start_time", &Event.start.time, "start_time/D");
    tree->Branch("end_time", &Event.end.time, "end_time/D");
    tree->Branch("start_position", &Event.start.position, "start_position/D");
    tree->Branch("end_position", &Event.end.position, "end_position/D");
    tree->Branch("start_bar", &Event.start.bar, "start_bar/I");
    tree->Branch("end_bar", &Event.end.bar, "end_bar/I");

    // Mở file txt để ghi dữ liệu
    std::ofstream outFile("outputtree.txt");

    // Lặp qua mỗi entry trong TTree và ghi giá trị của x, y, z, plane, bar, time và position vào file
    int entries = tree->GetEntries();
    for (int i = 0; i < entries; i++) {
        tree->GetEntry(i);

        // Gọi hàm getBarEnds để lấy tên Plane và Bar
        double startX1, startY1, startZ1, startX2, startY2, startZ2;
        double endX1, endY1, endZ1, endX2, endY2, endZ2;
        getBarEnds(Event.start.plane, Event.start.bar, startX1, startY1, startZ1, startX2, startY2, startZ2);
        getBarEnds(Event.end.plane, Event.end.bar, endX1, endY1, endZ1, endX2, endY2, endZ2);

        // In ra thông tin về Plane và Bar
        outFile << "Start: Plane = " << Event.start.plane << ", Bar = " << Event.start.bar << std::endl;
        outFile << "x = " << Event.start.x << ", y = " << Event.start.y << ", z = " << Event.start.z
                << ", time = " << Event.start.time << ", position = " << Event.start.position << std::endl;
        outFile << "End: Plane = " << Event.end.plane << ", Bar = " << Event.end.bar << std::endl;
        outFile << "x = " << Event.end.x << ", y = " << Event.end.y << ", z = " << Event.end.z
                << ", time = " << Event.end.time << ", position = " << Event.end.position << std::endl;
    }

    // Đóng file txt
    outFile.close();

    // Đóng tệp dữ liệu
    file->Close();
}