package keyGenerator;
import java.math.BigInteger;
import java.util.Random;

public class PairKeyGenerator 
{
	private BigInteger p;
	private BigInteger q;
	private BigInteger x;
	private BigInteger y;
	
	private void getPrimes()
	{
		long seed = System.currentTimeMillis();
		PrimeGenerator PG = new PrimeGenerator(seed);
		this.p = PG.getPrime();
		seed += 777;  // change random seed
		PG = new PrimeGenerator(seed);
		this.q = PG.getPrime();
	}
	
	public BigInteger[] getParameters()
	{
		this.getPrimes();
		BigInteger[] results = new BigInteger[3];  // n, e, d
		BigInteger euler = this.p.subtract(BigInteger.ONE).multiply(this.q.subtract(BigInteger.ONE));
		long seed = System.currentTimeMillis();
		Random random = new Random(seed);	
		// public key
		BigInteger e = new BigInteger(PrimeGenerator.keyLen, random);
		e = e.mod(euler.subtract(BigInteger.ONE)).add(BigInteger.ONE);
		while(!PairKeyGenerator.gcd(e, euler).equals(BigInteger.ONE))
		{
			e = new BigInteger(PrimeGenerator.keyLen, random);
			e = e.mod(euler.subtract(BigInteger.ONE)).add(BigInteger.ONE);
		}
		//System.out.println("e: " + e);
		//System.out.println("euler: " + euler);
		// private key
		BigInteger d = this.inv(e, euler);
		//if(d.modInverse(euler).equals(e))
		//	System.out.println("exgcd correct!");
		
		results[0] = this.p.multiply(this.q);
		results[1] = e;
		results[2] = d;
		return results;
	}
	
	private void exgcd(BigInteger a,BigInteger b)
	{
		//System.out.println("a: " + a);
		//System.out.println("b: " + b);
		// calc x,y
		if (b.equals(BigInteger.ZERO)) 
		{
			this.x = BigInteger.ONE;
			this.y = BigInteger.ZERO;
			return;
		}
		exgcd(b, a.mod(b));
		BigInteger x2 = this.x;
		BigInteger y2 = this.y;
		this.x = y2;
		this.y = x2.subtract(a.divide(b).multiply(y2));

	}
	
	public BigInteger inv(BigInteger a, BigInteger n)
	{
		// return a' (mod n)
		this.exgcd(a, n);
		return this.x.mod(n);
	}
	
	public static BigInteger gcd(BigInteger a, BigInteger b)
	{
		if(a.mod(b).equals(BigInteger.ZERO))
			return b;
		else
			return PairKeyGenerator.gcd(b, a.mod(b));
	}
	
	public static void main(String[] args) 
	{
		// TODO Auto-generated method stub
		PairKeyGenerator PKG = new PairKeyGenerator();
		BigInteger[] res = new BigInteger[3];
		res = PKG.getParameters();
		System.out.println(res[0]);
		System.out.println(res[1]);
		System.out.println(res[2]);
	}

}
