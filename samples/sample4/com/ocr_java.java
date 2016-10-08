package com;
import java.util.Arrays;

public class ocr_java {
    static {
        System.loadLibrary("ocr_java");
   }
	public native char[] getOcrArray(String srcImg);
	public native void setOcrArray(char[] ocrArray);
	public static void main(String[] args) {
		char[] array = new ocr_java().getOcrArray(args[0]);
		//System.out.println("Array is "+Arrays.toString(array));
		new ocr_java().setOcrArray(array);
	}
}
