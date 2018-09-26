package keyGenerator;
import java.math.BigInteger;
import java.util.Random;

public class PrimeGenerator 
{
	private BigInteger prime;
	private Random random;
	private static final int keyLen = 1024;
	private static final int s = 20;
	
	PrimeGenerator(int seed)
	{
		this.random = new Random(seed);
		
		
	}
	
	private boolean MR(BigInteger x)
	{
		// x - 1 = (2^t) * u
		BigInteger u = x.subtract(BigInteger.ONE);
		int t = 0;
		while(u.and(BigInteger.ONE).equals(BigInteger.ZERO))
		{
			++t;
			u = u.shiftRight(1);	
		}
		//System.out.println("u: " + u);
		//System.out.println("t: " + t);
		
		// start tests
		for(int i = 1; i <= s; ++i)
		{
			// rand a = 1 ~ x-1
			BigInteger a = new BigInteger(PrimeGenerator.keyLen, random);
			a = a.mod(x.subtract(BigInteger.ONE)).add(BigInteger.ONE);
			//System.out.println("a: " + a);
			BigInteger x0 = a.modPow(u, x);
			BigInteger x1 = x0;
			
			// test x by a
			for(int j = 1; j <= t; ++j)
			{
				x1 = x0.multiply(x0).mod(x);
				if(x1.equals(BigInteger.ONE) && !x0.equals(BigInteger.ONE) && !x0.equals(x.subtract(BigInteger.ONE)))
				{
					return false;
				}
				x0 = x1;
			}
			if(!x0.equals(BigInteger.ONE))
				return false;
		}
		return true;
	}
	
	public BigInteger getPrime()
	{
		/*int count = 0;
		while(true)
		{
			++count;
			// rand odd
			this.prime = new BigInteger(PrimeGenerator.keyLen, this.random);
			this.prime = this.prime.or(BigInteger.ONE.shiftLeft(PrimeGenerator.keyLen));
			this.prime = this.prime.or(BigInteger.ONE);
			System.out.println(count);
				
			// prime test
			if(this.MR(this.prime))
				break;
		}
		
		return this.prime;*/
		BigInteger tt = new BigInteger("25");  // 100101
		if(this.MR(tt))
			return tt;
		else
			return BigInteger.ZERO;
	}
	
	public static void main(String[] args) 
	{
		// TODO Auto-generated method stub
		PrimeGenerator test = new PrimeGenerator(25);
		System.out.println(test.getPrime());
		
	}
	
	

}
