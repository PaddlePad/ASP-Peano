import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JFrame;
import javax.swing.JPanel;

public class Paint extends JFrame {
	
	private final int SIZE = 729;
	private final int BORDER_OFFSET = 100;
	
	private Point2D[] field;
	private int grad;
	
	public Paint(Point2D[] field, int grad)
	{
		this.field = field;
		this.grad = grad;

		setTitle("Fuck Yeah!");
		setSize(1000, 1000);
		setBackground(Color.WHITE);
		setVisible(true);
		setDefaultCloseOperation(EXIT_ON_CLOSE);
	}

	public void paint(Graphics g)
	{
		int side = (int) Math.pow(3, grad);
		int quadLen = SIZE / side;
		int offset = quadLen / 2;
		
		int currX = BORDER_OFFSET;
		int currY = BORDER_OFFSET;
		
		for (int x = 0; x < side; x++) {
			for (int y = 0; y < side; y++) {
				g.drawRect(currX, currY, quadLen, quadLen);
				currY += quadLen;
			}
			
			currX += quadLen;
			currY = BORDER_OFFSET;
		}
		
//		Graphics2D curve = (Graphics2D)g;
//		curve.setStroke(new BasicStroke(2));
//		curve.setColor(Color.red);
//		
//		for (int i = 1; i < field.length; i++) {
//			
//			int xFrom = BORDER_OFFSET + offset + (field[i - 1].getX() - 1) * quadLen;
//			int yFrom = BORDER_OFFSET + reorient(offset + (field[i - 1].getY() - 1) * quadLen);
//			int xTo = BORDER_OFFSET + offset + (field[i].getX() - 1) * quadLen;
//			int yTo = BORDER_OFFSET + reorient(offset + (field[i].getY() - 1) * quadLen);
//			
//			curve.drawLine(xFrom, yFrom, xTo, yTo);
//		}
	}
	
	private int reorient(int y)
	{
		return SIZE - y;
	}
}
