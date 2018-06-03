public class factor8_1 {

    public static void main(String[] args) {
		System.out.println("Searching for two factors in range [1, 15000] whose multiple is 87654321");

		long startTime = System.nanoTime();
		for(int i=1; i<=15000; i++)
			for(int j=i+1; j<=15000; j++)
				if( i*j==87654321 )
					System.out.println("Found! "+i+' '+j);
		long endTime = System.nanoTime();
		double elapsedTime = (endTime-startTime)/1000000.;
		
		System.out.println("kernel execution time = "+elapsedTime+" msecs");
	}

}
