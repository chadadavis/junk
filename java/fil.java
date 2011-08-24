
/**
 * Describe class <code>fil</code> here.
 *
 * @author <a href="mailto:cad8@r172121.olydorf.swh.mhn.de">Chad Davis</a>
 * @version 1.0
 */


public class fil {
	
	/**
	 * Describe <code>main</code> method here.
	 *
	 * @param args a <code>String[]</code> value
	 */
	public static void main (String[] args) {
		for (int i = 0; i < args.length; i++) {
			String name = args[i];

			String shortName = name.substring(1);
			System.out.println("hey");
			System.out.println(name + "," + name + ", bo-B" + shortName);
			
			System.out.println("Banana-fana Fo-F" + shortName);
			System.out.println("Fee, Fie, mo-M" + shortName);
			System.out.println(name + "!");
		}
	}
}


