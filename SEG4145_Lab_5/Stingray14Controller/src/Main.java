import javax.swing.JFrame;

public class Main {
	public static void main (String []args){
		// Client c = new Client();
		Grid g = new Grid(/*c*/);
	    g.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	    g.pack();
	    g.setVisible(true);
	}	
}
