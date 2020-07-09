using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Madness.Engine
{
	/// <summary>
	/// Summary description for MainForm.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Panel viewPanel;
		private Madness.Engine.UserControls.Console conMain;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		protected override void OnPaint(System.Windows.Forms.PaintEventArgs e)
		{
			MadnessRender();
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.viewPanel = new System.Windows.Forms.Panel();
			this.conMain = new Madness.Engine.UserControls.Console();
			this.SuspendLayout();
			// 
			// viewPanel
			// 
			this.viewPanel.Location = new System.Drawing.Point(0, 0);
			this.viewPanel.Name = "viewPanel";
			this.viewPanel.Size = new System.Drawing.Size(1008, 624);
			this.viewPanel.TabIndex = 0;
			// 
			// conMain
			// 
			this.conMain.Location = new System.Drawing.Point(0, 624);
			this.conMain.Name = "conMain";
			this.conMain.Size = new System.Drawing.Size(1008, 112);
			this.conMain.TabIndex = 1;
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(1006, 724);
			this.Controls.Add(this.conMain);
			this.Controls.Add(this.viewPanel);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Name = "MainForm";
			this.Text = "MainForm";
			this.ResumeLayout(false);

		}
		#endregion

		[DllImport("Common.dll")]
		private static extern int MadnessInit(IntPtr hwnd, ConsoleCallBack ccb);
		[DllImport("Common.dll")]
		private static extern int MadnessRender();
		[DllImport("Common.dll")]
		private static extern int MadnessUpdate();
		[DllImport("Common.dll")]
		private static extern int MadnessCleanup();

		public delegate void ConsoleCallBack(IntPtr stringPtr);
		private static ConsoleCallBack CCB;
		private static MainForm frm;

		public static void ConsoleOutput(IntPtr stringPtr) 
		{
			string s = Marshal.PtrToStringAnsi(stringPtr);
			frm.conMain.Add(s);
			if(s == "SYSTEM[EXIT]") 
				Application.Exit();
		}

		[STAThread]
		static void Main()
		{
			frm = new MainForm();
			CCB = new ConsoleCallBack(ConsoleOutput);
			try 
			{
				MadnessInit(frm.viewPanel.Handle,CCB);
			} 
			catch(Exception ex) 
			{
				MessageBox.Show(ex.ToString());
			}
			Application.Run(frm);

			try 
			{
				MadnessCleanup();
			} 
			catch(Exception ex) 
			{
				MessageBox.Show(ex.ToString());
			}
		}
	}
}
