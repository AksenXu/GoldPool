./amrencode_test pcm.dat amr.dat

cat amr_header.dat amr.dat > amr.amr

ffmpeg -i amr.amr -ar 8000 amr_out.wav
