package keyGenerator;
import java.math.BigInteger;
import java.util.Random;

public class PrimeGenerator 
{
	// quite fast
	private BigInteger prime;
	private Random random;
	private static final int keyLen = 1024;
	private static final int s = 50;
	
	PrimeGenerator(int seed)
	{
		this.random = new Random(seed);
		
		
	}
	
	private BigInteger quickPowMod(BigInteger x, BigInteger n, BigInteger p)
	{
		// quick pow (mod p)
		BigInteger ans = new BigInteger("1");
		while(n.compareTo(BigInteger.ZERO) > 0)
		{
			if(n.and(BigInteger.ONE).equals(BigInteger.ONE))
				ans = ans.multiply(x).mod(p);
			x = x.multiply(x).mod(p);
			n = n.shiftRight(1);
		}
		
		return ans; 
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
			BigInteger x0 = this.quickPowMod(a, u, x);
			//BigInteger x0 = a.modPow(u, x);
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
		int count = 0;
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
		
		return this.prime;
		/*BigInteger tt = new BigInteger("25");  // 100101
		if(this.MR(tt))
			return tt;
		else
			return BigInteger.ZERO;*/
	}
	
	public static void main(String[] args) 
	{
		// TODO Auto-generated method stub
		PrimeGenerator test = new PrimeGenerator(98);
		System.out.println(test.getPrime());
		/*BigInteger a1 = new BigInteger("2");
		BigInteger a2 = new BigInteger("10");
		BigInteger a3 = new BigInteger("1020");
		System.out.println(test.quickPowMod(a1, a2, a3));*/
		
	}
	
	

}
