
int main()
{
    // get fps from video
    VideoCapture video("video.mp4");
    double fps = video.get(CAP_PROP_FPS);
}