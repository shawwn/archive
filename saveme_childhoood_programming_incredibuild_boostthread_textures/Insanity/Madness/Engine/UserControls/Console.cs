using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace Madness.Engine.UserControls
{
	/// <summary>
	/// Summary description for Console.
	/// </summary>
	public class Console : System.Windows.Forms.UserControl
	{
		private System.Windows.Forms.ListBox lbConsole;
		private System.Windows.Forms.TextBox txtConsole;
		private System.Windows.Forms.Button btnConsole;
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Console()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();

			// TODO: Add any initialization after the InitializeComponent call

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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.lbConsole = new System.Windows.Forms.ListBox();
			this.txtConsole = new System.Windows.Forms.TextBox();
			this.btnConsole = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lbConsole
			// 
			this.lbConsole.Location = new System.Drawing.Point(0, 0);
			this.lbConsole.Name = "lbConsole";
			this.lbConsole.Size = new System.Drawing.Size(1008, 82);
			this.lbConsole.TabIndex = 0;
			this.lbConsole.SelectedIndexChanged += new System.EventHandler(this.lbConsole_SelectedIndexChanged);
			// 
			// txtConsole
			// 
			this.txtConsole.Location = new System.Drawing.Point(0, 80);
			this.txtConsole.Name = "txtConsole";
			this.txtConsole.Size = new System.Drawing.Size(952, 20);
			this.txtConsole.TabIndex = 1;
			this.txtConsole.Text = "";
			this.txtConsole.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
			// 
			// btnConsole
			// 
			this.btnConsole.Location = new System.Drawing.Point(952, 80);
			this.btnConsole.Name = "btnConsole";
			this.btnConsole.Size = new System.Drawing.Size(56, 24);
			this.btnConsole.TabIndex = 2;
			this.btnConsole.Text = "Submit";
			this.btnConsole.Click += new System.EventHandler(this.btnConsole_Click_1);
			// 
			// Console
			// 
			this.Controls.Add(this.btnConsole);
			this.Controls.Add(this.txtConsole);
			this.Controls.Add(this.lbConsole);
			this.Name = "Console";
			this.Size = new System.Drawing.Size(1008, 104);
			this.ResumeLayout(false);

		}
		#endregion

		private void textBox1_TextChanged(object sender, System.EventArgs e)
		{
		
		}

		[DllImport("Common.dll")]
		private static extern int MadnessCommand(string str);

		public void Add(string text)
		{
			lbConsole.Items.Add(text);
			lbConsole.SetSelected(lbConsole.Items.Count-1,true);
		}

		private void btnConsole_Click_1(object sender, System.EventArgs e)
		{
			MadnessCommand(txtConsole.Text);
			txtConsole.Clear();
		}

		private void lbConsole_SelectedIndexChanged(object sender, System.EventArgs e)
		{
		
		}
	}
}
