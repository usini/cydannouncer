// pixel drawing callback
static int drawMCU(JPEGDRAW *pDraw)
{
    // Serial.printf("Draw pos = (%d, %d), size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
    unsigned long s = millis();
    gfx->draw16bitRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
    total_show_video_ms += millis() - s;
    return 1;
} /* drawMCU() */

bool open_video(String filename)
{
    bool status = false;
    String aac_filename = filename + ".aac";
    Serial.println("Open AAC file: " + aac_filename);
    gfx->println("Open AAC file: " + aac_filename);
    File aFile = SD.open(aac_filename);

    if (!aFile || aFile.isDirectory())
    {
        Serial.println("ERROR: Failed to open " + aac_filename + " file for reading");
        gfx->println("ERROR: Failed to open " + aac_filename + " file for reading");
        return false;
    }
    else
    {
        String mjpeg_filename = filename + ".mjpeg";
        Serial.println("Open MJPEG file: " + mjpeg_filename);
        gfx->println("Open MJPEG file: " + mjpeg_filename);
        File vFile = SD.open(mjpeg_filename);
        if (!vFile || vFile.isDirectory())
        {
            Serial.println("ERROR: Failed to open " + mjpeg_filename + " file for reading");
            gfx->println("ERROR: Failed to open " + mjpeg_filename + " file for reading");
            return false;
        }
        else
        {
            Serial.println("Init video");
            gfx->println("Init video");
            mjpeg_setup(&vFile, MJPEG_BUFFER_SIZE, drawMCU,
                        false /* useBigEndian */, DECODEASSIGNCORE, DRAWASSIGNCORE);

            Serial.println("Start play audio task");
            gfx->println("Start play audio task");
            BaseType_t ret_val;

            ret_val = aac_player_task_start(&aFile, AUDIOASSIGNCORE);

            if (ret_val != pdPASS)
            {
                Serial.printf("Audio player task start failed: %d\n", ret_val);
                gfx->printf("Audio player task start failed: %d\n", ret_val);
                return false;
            }

            Serial.println("Start play video");
            gfx->println("Start play video");
            start_ms = millis();
            curr_ms = millis();
            next_frame_ms = start_ms + (++next_frame * 1000 / FPS / 2);
            while (vFile.available() && mjpeg_read_frame()) // Read video
            {
                total_read_video_ms += millis() - curr_ms;
                curr_ms = millis();

                if (millis() < next_frame_ms) // check show frame or skip frame
                {
                    // Play video
                    mjpeg_draw_frame();
                    total_decode_video_ms += millis() - curr_ms;
                    curr_ms = millis();
                }
                else
                {
                    ++skipped_frames;
                    Serial.println("Skip frame");
                }

                while (millis() < next_frame_ms)
                {
                    vTaskDelay(pdMS_TO_TICKS(1));
                }

                curr_ms = millis();
                next_frame_ms = start_ms + (++next_frame * 1000 / FPS);
            }
            int time_used = millis() - start_ms;
            int total_frames = next_frame - 1;
            Serial.println("AV end");
            vFile.close();
            aFile.close();
            return true;
        }
    }
    return false;
}