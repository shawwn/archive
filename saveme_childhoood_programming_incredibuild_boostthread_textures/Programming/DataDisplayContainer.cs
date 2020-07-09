using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace CommonSharpControls.Forms
{
	/// <summary>
	/// Summary description for DataDisplayContainer.
	/// </summary>
	public class DataDisplayContainer : System.Windows.Forms.Form
	{
		private CommonSharpControls.Controls.DataDisplay dataDisplay;
		private System.Windows.Forms.Button btnClose;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public DataDisplayContainer()
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

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.dataDisplay = new CommonSharpControls.Controls.DataDisplay();
			this.btnClose = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// dataDisplay
			// 
			this.dataDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.dataDisplay.Location = new System.Drawing.Point(0, 0);
			this.dataDisplay.Name = "dataDisplay";
			this.dataDisplay.Size = new System.Drawing.Size(560, 472);
			this.dataDisplay.TabIndex = 0;
			// 
			// btnClose
			// 
			this.btnClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.btnClose.Location = new System.Drawing.Point(232, 480);
			this.btnClose.Name = "btnClose";
			this.btnClose.Size = new System.Drawing.Size(88, 32);
			this.btnClose.TabIndex = 1;
			this.btnClose.Text = "Close";
			this.btnClose.Click += new System.EventHandler(this.btnClose_Click);
			// 
			// DataDisplayContainer
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.CancelButton = this.btnClose;
			this.ClientSize = new System.Drawing.Size(552, 517);
			this.Controls.Add(this.btnClose);
			this.Controls.Add(this.dataDisplay);
			this.KeyPreview = true;
			this.Name = "DataDisplayContainer";
			this.Text = "Realtime Variable Display";
			this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.DataDisplayContainer_KeyPress);
			this.ResumeLayout(false);

		}
		#endregion

		private void DataDisplayContainer_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			int c = e.KeyChar;
			switch(c)
			{
				case 27:
					Hide();
					break;
			}
		}

		public void UpdateData(string Name, string Value)
		{
			dataDisplay.ChangeData(Name,Value);
		}

		public void RemoveData(string Name)
		{
			dataDisplay.RemoveData(Name);
		}

		public void ParseData(string args)
		{
			ArrayList arglist = new ArrayList();
			int sel = args.IndexOf(" ");
			string arg = "";
			int count = 0;
			while(sel != -1) 
			{
				arg = args.Substring(0,sel);
				args = args.Substring(sel+1);
				arglist.Add(arg);
				sel = args.IndexOf(" ");
				count++;
				if(count == 2)
					break;
			}
			arglist.Add(args);
			for(int i = 0; i < arglist.Count;i++) 
			{
				if(arglist[i].ToString() == "update") 
				{
					UpdateData(arglist[i+1].ToString(),arglist[i+2].ToString());
					break;
				}
				if(arglist[i+1].ToString() == "remove") 
				{
					RemoveData(arglist[i+1].ToString());
					break;
				}
			}
		}

		private void btnClose_Click(object sender, System.EventArgs e)
		{
			Hide();
		}
	}
}
