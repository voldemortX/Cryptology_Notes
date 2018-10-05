package encryptor;


import java.io.FileOutputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.io.FileInputStream;
import java.io.FileNotFoundException;

import keyGenerator.PairKeyGenerator;
import keyGenerator.PrimeGenerator;

public class Encryptor 
{
	public static void generateNewKeys(String privateFilename, String publicFilename) throws IOException
	{
		// generate and store a new key pair
		PairKeyGenerator PKG = new PairKeyGenerator();
		BigInteger[] res = new BigInteger[3];
		res = PKG.getParameters();
		String pub = "n:" + res[0].toString() + " key:" + res[1].toString();
		String pri = "n:" + res[0].toString() + " key:" + res[2].toString();
		
		try 
		{
			FileOutputStream fout = new FileOutputStream(privateFilename);
			fout.write(pri.getBytes());
	        fout.close();
		} 
		catch (FileNotFoundException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try 
		{
			FileOutputStream fout = new FileOutputStream(publicFilename);
			fout.write(pub.getBytes());
	        fout.close();
		} 
		catch (FileNotFoundException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		//System.out.println(res[0]);
		//System.out.println(res[1]);
		//System.out.println(res[2]);
	}
	
	public static boolean encrypt(String inputFilename, String outputFilename, String keyFilename) throws IOException
	{
		// decryption is exactly the same
		BigInteger input = null;
		BigInteger output = null;
		BigInteger n = null;
		BigInteger key = null;
		final int maxLen = 257;  // long messages will be cut(consider "BigInteger" sign bit)
		
		try 
		{
			FileInputStream fin = new FileInputStream(keyFilename);
			byte[] content = new byte[fin.available()];
			fin.read(content);
			fin.close();
			String keyFiled = new String(content);
			int s1, e1, s2;
			if(keyFiled.indexOf("n:") == -1 || keyFiled.indexOf("key:") == -1)
				return false;
			s1 = keyFiled.indexOf("n:") + 2;
			e1 = keyFiled.indexOf("key:") - 1;
			s2 = keyFiled.indexOf("key:") + 4;
			n = new BigInteger(keyFiled.substring(s1, e1));
			key = new BigInteger(keyFiled.substring(s2));
			
		} 
		catch (FileNotFoundException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try 
		{
			FileInputStream fin = new FileInputStream(inputFilename);
			byte[] content = new byte[fin.available()];
			fin.read(content);
			//System.out.println("sign: " + content[0]);
			//content[0] = 0;
			fin.close();
			byte[] digested = new byte[maxLen - 1];  // figuratively
			if(content.length > maxLen)
			{
				System.arraycopy(content, 0, digested, 0, maxLen - 1);
				//System.out.println("d0: " + digested[0]);
				input = new BigInteger(digested);
				System.out.println("message is bigger than 256B, digestion(cut) occured!");
			}
			else
				input = new BigInteger(content);
			if(input.compareTo(n) > 0 || input.add(n).compareTo(BigInteger.ZERO) < 0)
			{
				input = input.mod(n);  // fail-safe
				System.out.println("message is bigger than n, digestion(mod) occured!");
			}
			
		} 
		catch (FileNotFoundException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		// java has no unsigned types, frak!
		boolean negative = false;
		if(input.compareTo(BigInteger.ZERO) < 0)
			negative = true;
		output = PrimeGenerator.quickPowMod(input, key, n);  // encryption
		if(negative)
			output = output.subtract(n);
		
		//System.out.println("input: " + input);
		//System.out.println("key: " + key);
		//System.out.println("n: " + n);
		//System.out.println("output: " + output);
		//System.out.println("output_minus" + output.subtract(n));
		
		try 
		{
			FileOutputStream fout = new FileOutputStream(outputFilename);
			fout.write(output.toByteArray());
	        fout.close();
		} 
		catch (FileNotFoundException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return true;
	}
	
	public static void main(String[] args) 
	{
		// TODO Auto-generated method stub
		try 
		{
			Encryptor.generateNewKeys("rsa.txt", "rsa_pub.txt");
		} 
		catch (IOException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try 
		{
			boolean status = Encryptor.encrypt("m.txt", "en.txt", "rsa_pub.txt");
			if(status)
				System.out.println("calculation completed!");
		} 
		catch (IOException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try 
		{
			boolean status = Encryptor.encrypt("en.txt", "de.txt", "rsa.txt");
			if(status)
				System.out.println("calculation completed!");
		} 
		catch (IOException e) 
		{
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}
