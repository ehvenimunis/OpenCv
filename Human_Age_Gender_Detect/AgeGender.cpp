/*
* Aurhor : Muhammed Salih Aydoğan
* Date   : 25/08/2020
* Topic  : Derin Öğrenmeyi kullanarak Yaş Cinsiyet tanıma kodu
* Result : Okunan Resim - images/Elon_Musk.jpg
           Cinsiyet: Male
           Yaş: (48-53)

           Okunan Resim - images/Etki.jpg
           Cinsiyet: Female
           Yaş: (0-2)

           Okunan Resim - images/Recep_Tayyip_Erd.jpg
           Cinsiyet: Male
           Yaş: (60-100)

           Okunan Resim - images/fatih-altayli.jpg
           Cinsiyet: Male
           Yaş: (60-100)

           Okunan Resim - images/iha.jpg
           Cinsiyet: Male
           Yaş: (25-32)
*/

#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <iterator>
#include <tuple>

using namespace cv;
using namespace cv::dnn;
using namespace std;
 
/*
  Yüz algılama için DNN Yüz Dedektörünü kullanacağız.
  Yüz algılama, aşağıda gösterildiği gibi getFaceBox işlevi kullanılarak yapılır.
*/
tuple<Mat, vector<vector<int>>> getFaceBox(Net net, Mat &frame, double conf_threshold)
{
    Mat frameOpenCVDNN = frame.clone();
    int frameHeight = frameOpenCVDNN.rows;
    int frameWidth = frameOpenCVDNN.cols;
    double inScaleFactor = 1.0;
    Size size = Size(300, 300);

    Scalar meanVal = Scalar(104, 117, 123);

    cv::Mat inputBlob;
    inputBlob = cv::dnn::blobFromImage(frameOpenCVDNN, inScaleFactor, size, meanVal, true, false);

    net.setInput(inputBlob, "data");
    cv::Mat detection = net.forward("detection_out");

    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    vector<vector<int>> bboxes;

    for(int i = 0; i < detectionMat.rows; i++)
    {
        float confidence = detectionMat.at<float>(i, 2);

        if(confidence > conf_threshold)
        {
            int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
            int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
            int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
            int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);
            vector<int> box = {x1, y1, x2, y2};
            bboxes.push_back(box);
            cv::rectangle(frameOpenCVDNN, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 255, 0),2, 4);
        }
    }
    return make_tuple(frameOpenCVDNN, bboxes);
}

int main(int argc, char** argv)
{
  // Kullanılan hazır model
  string faceProto = "opencv_face_detector.pbtxt";
  string faceModel = "opencv_face_detector_uint8.pb";

  string ageProto = "age_deploy.prototxt";
  string ageModel = "age_net.caffemodel";

  string genderProto = "gender_deploy.prototxt";
  string genderModel = "gender_net.caffemodel";

  Scalar MODEL_MEAN_VALUES = Scalar(78.4263377603, 87.7689143744, 114.895847746);

  vector<string> ageList = {"(0-2)", "(4-6)", "(8-12)", "(15-20)", "(25-32)",
    "(38-43)", "(48-53)", "(60-100)"};

  vector<string> genderList = {"Male", "Female"};

  Net ageNet = readNet(ageModel, ageProto);  // Bu işlev, eğitilmiş modelin başlangıç çerçevesini otomatik olarak algılar
  Net genderNet = readNet(genderModel, genderProto);
  Net faceNet = readNet(faceModel, faceProto);

  /////////////////////////------------------->
	
	//Görüntü yolunu depolamak için bir dizi vektörü bildirin
	vector<cv::String> fn;
	glob("images/*.jpg", fn, false);

	// Vektör Bildirimi
	vector<Mat> images;
	
	// Görüntüler arasında yineleme yapmak için count değişkeninde vektör boyutunu depolamak
	size_t count = fn.size(); //image klasöründeki png dosyalarının sayısı

	// Mat nesnesine girdi olarak resim almak için imread işlevi kullanıldı
	for (size_t i=0; i<count; i++)
	{ images.push_back(imread(fn[i]));
	
  vector<vector<int>> bboxes;
  Mat frameFace;
      
  // Her seferinde giriş olarak 1 görüntü alma ve frame içinde saklama
  Mat frame = images[i];
	  
  //////////////////////////////--------------->
     
  tie(frameFace, bboxes) = getFaceBox(faceNet, frame, 0.7);
    
  // Halihazırda yüklü olan görüntünün adını yazdırma
  cout<<endl<<"Okunan Resim - "<<fn[i]<<endl;


  if(bboxes.size() == 0) {
    cout << "Yüz algılanmadı, sonraki kare kontrol ediliyor." << endl;
    continue;
  }

  int padding=20;
  for (auto it = begin(bboxes); it != end(bboxes); ++it) {
    Rect rec(it->at(0) - padding, it->at(1) - padding, it->at(2) - it->at(0) + 2*padding, it->at(3) - it->at(1) + 2*padding);
    Mat face = frame(rec); // take the ROI of box on the frame

    Mat blob;
    blob = blobFromImage(face, 1, Size(227, 227), MODEL_MEAN_VALUES, false);
    genderNet.setInput(blob);
    // string gender_preds;
    vector<float> genderPreds = genderNet.forward();
    // printing gender here
    // find max element index
    // distance function does the argmax() work in C++
    int max_index_gender = std::distance(genderPreds.begin(), max_element(genderPreds.begin(), genderPreds.end()));
    string gender = genderList[max_index_gender];

    cout << "Cinsiyet: " << gender << endl;

    ageNet.setInput(blob);
    vector<float> agePreds = ageNet.forward();

    // age_preds vektöründe maksimum indis bulma
    int max_indice_age = std::distance(agePreds.begin(), max_element(agePreds.begin(), agePreds.end()));
    string age = ageList[max_indice_age];
    cout << "Yaş: " << age << endl;
    string label = gender + ", " + age; // etiket

    // Herhangi bir hata olması durumunda try catch'in uygulanması
    try
    {
      cv::putText(frameFace, label, Point(it->at(-1), it->at(1) -15), cv::FONT_HERSHEY_SIMPLEX, 0.9, Scalar(0, 255, 255), 2, cv::LINE_AA);
      imshow("Frame", frameFace);
            imwrite("out.jpg",frameFace);
    }
    catch(...)
    {
      
    }
  }
  }
} // main fonksiyonu sonu 


