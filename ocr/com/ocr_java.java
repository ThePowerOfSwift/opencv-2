package com;

public class ocr_java {
    static {
        System.loadLibrary("ocr_java");
   }
	public native int OcrPreprocess(String srcImg, String desImg);
	public native int OcrTesseract(String input, String output, String ocr_type);
	public static void main(String[] args) {
		//new ocr_java().OcrPreprocess(args[0], args[1]);
		new ocr_java().OcrTesseract(args[1], args[2], "eng");
	}
}
