package com;

public class ocr_java {
    static {
        System.loadLibrary("ocr_java");
   }
	public native String getOcrArray(String srcImg);
	public static void main(String[] args) {
		String desImg = new ocr_java().getOcrArray(args[0]);
		System.out.println("Base64 encode is "+ desImg);
	}
}
