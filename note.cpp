
int main()
{
    // get fps from video
    VideoCapture video("video.mp4");
    double fps = video.get(CAP_PROP_FPS);
}

while (true) {
    unsigned char buffer [4096] = {};

    for (size_t nbuffer = 0; nbuffer < sizeof buffer; nbuffer = MAX(nbuffer, sizeof buffer)) { /* Watch out for buffer overflow */
        int len = recv (sockd, buffer, sizeof buffer, 0);

        /* FIXME: Error checking */

        nbuffer += len;
    }

    /* We have a whole chunk, process it: */
    ;
}