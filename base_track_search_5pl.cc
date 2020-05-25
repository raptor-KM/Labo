#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <errno.h>
#include <iomanip>
#include <ios>
#include <errno.h>
#include <map>
#include <tuple>
#include <getopt.h>



// failの有無の評価
bool file_exists(const std::string& f_name){
	std::ifstream fs(f_name);
	return fs.is_open();
}

// 帰ってきた答えが正しいかの評価
bool Ans_judge(std::string& answer){
	if( answer=="y" || answer=="Y" || answer=="n" || answer=="N"){
		return true;
	}
	else{
		return false;
	}
}

// yes or noの評価
bool Y_N_bool(std::string& answer){
	if( answer=="y" || answer=="Y"){
		return true;
	}
	else{
		return false;
	}
}

void options_overlap_Judge(std::string str, std::map<std::string, std::string> opt){
	std::cout << "An option that cannot be specified at the same time is specified." << std::endl;
}

void origin_mfile_output(std::string mfile_name, std::ofstream &fout){

	std::ifstream fin;
	std::string str;

	fin.open(mfile_name);
	if(!fin){
		std::cerr << " no exist filepath : " << strerror(errno) << std::endl;
		std::cerr << " print error code : " << errno << std::endl;
		std::cerr << " can not open " << mfile_name <<  std::endl;
		exit(1);
	}

	while (getline(fin,str)){
		fout << str << std::endl;
	}

	fin.close();
}


void usage(int arg_numb,  char *argument[]){

	if (arg_numb<2) {
		std::cout << std::endl;
		std::cout << "usage : /data2/T60/run6/work/mizuno/work/191204/base_track_search_5pl -J [-m] -E -A [--en] --PL [--tID] [--phvph] [--Ang] --xyz [-r] [--Ad] [--tol] [--outf]" << std::endl;
		std::cout << "usage : /data2/T60/run6/work/mizuno/work/191204/base_track_search_5pl -J [-m] -E -A [--en] --all [-r] [--Ad] [-tol] [-outf]" << std::endl;
		std::cout << "usage : /data2/T60/run6/work/mizuno/work/191204/base_track_search_5pl -pf" << std::endl;
		std::cout << "[-J     : Judge criteria]      :   Criteria for track. ip or md                                                                               ※Required" << std::endl;
		std::cout << "[-m     : Original mfile]      :   mfile that contains track information you absolutely want to write.                                        not required" << std::endl;
		std::cout << "[-E     : ECC]                 :   ECC number to search for base track.                                                                       ※Required" << std::endl;
		std::cout << "[-A     : Area]                :   Area number to search for base track.                                                                      ※Required" << std::endl;
		std::cout << "[-en    : event number]        :   The Original track event number.                                                                           not required" << std::endl;
		std::cout << "[-PL    : PL]                  :   The Original track PL number.                                                                              ※Required" << std::endl;
		std::cout << "[-tID   : track ID]            :   The Original track track ID.                                                                               not required" << std::endl;
		std::cout << "[-phvph : phvph]               :   The Original track phvph.                                                                                  not required" << std::endl;
		std::cout << "[-Ang   : Reference angle]     :   The Original track angle.   -Ang ax ay                                                                     not required" << std::endl;
		std::cout << "[-xyz   : Reference xyz]       :   The Original track x,y,z.   -xyz x y z                                                                     ※Required" << std::endl;
		std::cout << "[-r     : radius]              :   The radius of the circle when cutting the track into a cylinder.                                           not required" << std::endl;
		std::cout << "[-Ad    : Angle difference]    :   The angle difference allowed when the track is cut into a cylinder or corn.                                not required" << std::endl;
		std::cout << "[-tol   : tolerance]           :   The allowable range of md vertex point.                                                                    not required" << std::endl;
		std::cout << "[-outf  : output file name]    :   output mfile name. If this is not set, it will be s00*_md200_PL_track_ID_md_or_ip_follow_up_down.all.      not required" << std::endl;
		std::cout << "[-pf    : parameter file name] :   All arguments can be written to this file. Refer to \"/data2/T60/run6/work/mizuno/work/191204/prmfile.dat\"  not required" << std::endl;
		std::cout<< "[-all   : original track prm]  :   Set \"E A  event_number PL track_ID phphv ax ay x y z\" at once.                                             ※Either a required parameter or this is required." << std::endl;
		std::cout << "The values when none of \"radius ,angle difference and tolerance\" are specified are \"50 ,0.3 and 400\". " << std::endl;
		std::cout << std::endl;
		std::cout << "(note) " << std::endl;
		std::cout << " This program uses c++11. " << std::endl;
		std::cout << " Don't forget to add \"-std=c++11\" when compiling. " << std::endl;

		std::cout << std::endl;

		exit (1);
	}

}

void safety_device(int argc, int index, std::string opt_Label){
	if(index>=argc){
		std::cout << opt_Label << " is strenge." <<std::endl;
		std::cout << "Please review" << std::endl;
		exit(1);
	}
}

struct same_err{

	std::vector<std::string> arg_same_val;
	std::vector<std::string> arg_same_name[2];
	bool same = false;

	void err_Judge(int& optind, std::string opt_val, std::string opt_Label, std::string opt1, std::string opt2, std::map<std::string, std::string> &arg_analysis, bool FRAG, int argc){
		if( arg_analysis.find(opt_Label) == arg_analysis.end()) {
			arg_analysis[opt_Label] = opt_val;
			if (FRAG) {
				optind++;
			}
		}
		else {
			arg_same_val.push_back(opt_Label);
			arg_same_name[0].push_back(opt1);
			arg_same_name[1].push_back(opt2);
			same = true;
			if (FRAG) {
				optind++;
			}
		}
	}
};


void anal_arg(int argc, char *argv[], int &opt_val, const char *optstring, const struct option *longopts, int &longindex,std::map<std::string, std::string> &arg_analysis,struct same_err &Judgement_process, bool &miss_opt, bool &unknown){
	optind = 1;
	while((opt_val = getopt_long(argc, argv, optstring, longopts, &longindex)) != -1){
		switch (opt_val){
		case 'f':
			arg_analysis["parameter_file_name"] = optarg;
			break;
		case 'J':
			arg_analysis["Judge_criteria"] = optarg;
			break;
		case 'm':
			arg_analysis["Original_mfile"] = optarg;
			break;
		case 'E':
			arg_analysis["ECC"] = optarg;
			break;
		case 'A':
			arg_analysis["Area"] = optarg;
			break;
		case 'e':
			arg_analysis["event_number"] = optarg;
			break;
		case 'P':
			Judgement_process.err_Judge(optind,       optarg,       "PL", "-all",    "-PL", arg_analysis, false, argc);
			break;
		case 'I':
			Judgement_process.err_Judge(optind,       optarg, "track_ID", "-all",   "-tID", arg_analysis, false, argc);
			break;
		case 'p':
			Judgement_process.err_Judge(optind,       optarg,    "phvph", "-all", "-phvph", arg_analysis, false, argc);
			break;
		case 'a':
			Judgement_process.err_Judge(optind,       optarg,   "Ref_ax", "-all",  "-tAng", arg_analysis, false, argc);
			safety_device(argc, optind, "Ref_ay");
			Judgement_process.err_Judge(optind, argv[optind],   "Ref_ay", "-all",  "-tAng", arg_analysis,  true, argc);
			break;
		case 'x':
			Judgement_process.err_Judge(optind,       optarg,    "Ref_x", "-all",  "-txyz", arg_analysis, false, argc);
			safety_device(argc, optind, "Ref_y");
			Judgement_process.err_Judge(optind, argv[optind],    "Ref_y", "-all",  "-txyz", arg_analysis,  true, argc);
			safety_device(argc, optind, "Ref_z");
			Judgement_process.err_Judge(optind, argv[optind],    "Ref_z", "-all",  "-txyz", arg_analysis,  true, argc);
			break;
		case 'r':
			arg_analysis["radius"] = optarg;
			break;
		case 'd':
			arg_analysis["Accept_angle_difference"] = optarg;
			break;
		case 't':
			arg_analysis["tolerance"] = optarg;
			break;
		case 'o':
			arg_analysis["output_file_name"] = optarg;
			break;
		case 'L':
			Judgement_process.err_Judge(optind,       optarg,       "PL", "-all",    "-PL", arg_analysis, false, argc);
			safety_device(argc, optind, "track_ID");
			Judgement_process.err_Judge(optind, argv[optind], "track_ID", "-all",   "-tID", arg_analysis,  true, argc);
			safety_device(argc, optind, "phvph");
			Judgement_process.err_Judge(optind, argv[optind],    "phvph", "-all", "-phvph", arg_analysis,  true, argc);
			safety_device(argc, optind, "Ref_ax");
			Judgement_process.err_Judge(optind, argv[optind],   "Ref_ax", "-all",  "-tAng", arg_analysis,  true, argc);
			safety_device(argc, optind, "Ref_ay");
			Judgement_process.err_Judge(optind, argv[optind],   "Ref_ay", "-all",  "-tAng", arg_analysis,  true, argc);
			safety_device(argc, optind, "Ref_x");
			Judgement_process.err_Judge(optind, argv[optind],    "Ref_x", "-all",  "-txyz", arg_analysis,  true, argc);
			safety_device(argc, optind, "Ref_y");
			Judgement_process.err_Judge(optind, argv[optind],    "Ref_y", "-all",  "-txyz", arg_analysis,  true, argc);
			safety_device(argc, optind, "Ref_z");
			Judgement_process.err_Judge(optind, argv[optind],    "Ref_z", "-all",  "-txyz", arg_analysis,  true, argc);
			break;
		default:
			if(miss_opt){
				std::cout << "" << std::endl;
				std::cout << " The argument option entered is incorrect. " << std::endl;
				std::cout << " You have entered an option that has not been set. " << std::endl;
				std::cout << " Enter the correct option again. " << std::endl;
				std::cout << " Here are the options you entered incorrectly : " << std::endl;
				miss_opt = false;
			}
			unknown = true;
			break;
		}
	}
}


std::map<std::string, std::string> opt_process(int argc, char *argv[]){

	int opt_val;
	std::string arg;
	int longindex = 0;
	std::string str;
	std::map<std::string, std::string> arg_analysis;
	struct same_err Judgement_process;
	bool unknown = false;
	bool miss_opt = true;
	bool check_req = false;
	std::ifstream ifpr;
	int para_argc=1;
	std::vector<std::string> para_box;

	usage(argc, argv);

	const char* optstring = "J:m:E:A:r::";
	const struct option longopts[] = {
		//              *name   has_arg          *flag  val
		{       "en",      required_argument,  NULL, 'e'},
		{       "PL",      required_argument,  NULL, 'P'},
		{      "tID",      required_argument,  NULL, 'I'},
		{    "phvph",      required_argument,  NULL, 'p'},
		{      "Ang",      required_argument,  NULL, 'a'},
		{      "xyz",      required_argument,  NULL, 'x'},
		{       "Ad",      required_argument,  NULL, 'd'},
		{      "tol",      required_argument,  NULL, 't'},
		{     "outf",      required_argument,  NULL, 'o'},
		{       "pf",      required_argument,  NULL, 'f'},
		{      "all",      required_argument,  NULL, 'L'},
		{          0,                      0,         0,   0},
	};

	anal_arg(argc, argv, opt_val, optstring, longopts, longindex, arg_analysis,Judgement_process, miss_opt, unknown);

	if(arg_analysis.find("parameter_file_name") != arg_analysis.end()){
		ifpr.open(arg_analysis["parameter_file_name"]);
		if(!ifpr){
			std::cerr << " no exist filepath : " << strerror(errno) << std::endl;
			std::cerr << " print error code : " << errno << std::endl;
			std::cerr << " can not open " << arg_analysis["parameter_file_name"] <<  std::endl;
			exit(1);
		}
		para_box.push_back(arg_analysis["parameter_file_name"]);
		while (getline(ifpr,str)) {
			if(str[0] == '-'){
				para_box.push_back(str);
				while(str[0] != '#' && getline(ifpr, str)){
					if(str[0] != '#'){
						para_box.push_back(str);
					}
				}
			}
		}
		para_argc = para_box.size();
		char *para_argv[para_argc];
		for (int i = 0; i < para_argc; ++i) {
			para_argv[i] = (char *)malloc(sizeof(char) * (strlen(para_box[i].c_str())+1));
			strcpy(para_argv[i], para_box[i].c_str());
		}
		longindex = 0;
		anal_arg(para_argc, para_argv, opt_val, optstring, longopts, longindex, arg_analysis,Judgement_process, miss_opt, unknown);
	}


	if(unknown){
		std::cout << std::endl;
		exit(1);
	}

	struct checklst_t {
		std::string key;
		std::string msg;
		std::string status;

		int bool_Required(std::string str){
			if(str == "Required"){
				return 0;
			}
			else if(str == "not_Required"){
				return 1;
			}
			else{
				// Depending on the case ( -J = md)
				return 2;
			}
		}

	} checklist[] = {
		{         "Judge_criteria", "             -J  :  Criteria for track. ip or md."                                                                                , "Required"},
		{         "Original_mfile", "             -m  :  mfile that contains track information you absolutely want to write."                                          , "not_Required"},
		{                    "ECC", "             -E  :  ECC number to search for base track."                                                                         , "Required"},
		{                   "Area", "             -A  :  Area number to search for base track"                                                                         , "Required"},
		{           "event_number", "            -en  :  The Original track event number."                                                                             , "not_Required"},
		{                     "PL", "    -PL or -all  :  The Original track PL number."                                                                                , "Required"},
		{               "track_ID", "   -tID or -all  :  The Original track track ID."                                                                                 , "not_Required"},
		{                  "phvph", " -phvph or -all  :  The Original track phvph."                                                                                    , "not_Required"},
		{                 "Ref_ax", "   -Ang or -all  :  The Original track ax."                                                                                       , "Depending_on_the_case"},
		{                 "Ref_ay", "   -Ang or -all  :  The Original track ay."                                                                                       , "Depending_on_the_case"},
		{                  "Ref_x", "   -xyz or -all  :  The Original track x."                                                                                        , "Required"},
		{                  "Ref_y", "   -xyz or -all  :  The Original track y."                                                                                        , "Required"},
		{                  "Ref_z", "   -xyz or -all  :  The Original track z."                                                                                        , "Required"},
		{                 "radius", "             -r  :  The radius of the circle when cutting the track into a cylinder."                                             , "not_Required"},
		{"Accept_angle_difference", "            -Ad  :  The angle difference allowed when the track is cut into a cylinder or corn."                                  , "not_Required"},
		{              "tolerance", "           -tol  :  The allowable range of md vertex point."                                                                      , "not_Required"},
		{       "output_file_name", "          -outf  :  output mfile name. If this is not set, it will be s00*_md200_PL_track_ID_md_or_ip_follow_up_down.all."        , "not_Required"},
		{    "parameter_file_name", "            -pf  :  All arguments can be written to this file. Refer to \"/data2/T60/run6/work/mizuno/work/191203/prmfile.dat\"." , "not_Required"},
	};
	for(auto &item : checklist) {
		if(item.bool_Required(item.status)==2 && arg_analysis["Judge_criteria"]=="md" ){
			item.status = "Required";
		}
		if(arg_analysis.find(item.key) == arg_analysis.end()) {
			arg_analysis[item.key] = "non_argument";
			if(item.bool_Required(item.status)==0){
				std::cerr << "Error: " << item.key << " not found." << std::endl;
				std::cerr << item.msg << "   ※ " << item.status << std::endl;
				check_req = true;
			}
		}
	}
	if(check_req){
		std::cout << std::endl;;
		exit(1);
	}
	return arg_analysis;
}


// check PLの諸々
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::tuple< int, int ,int ,std::string > brick_PL_relation_various(int Stan_PL){

	int max;
	int min;
	std::string brick;
	int check_PL;

	if( 11 <= Stan_PL & Stan_PL <= 231){
		max = 231;
		min = 11;
		brick = std::string( "0123" );

		if( Stan_PL-50<min ){
			check_PL = 11 - (int)((50 - Stan_PL + min)/10);
		}
		else if( Stan_PL+50>max ){
			check_PL = 11 - (int)((Stan_PL + 50 - max)/10);
		}
		else{
			check_PL =11;
		}
	}
	else if(Stan_PL <= 461){
		max = 461;
		min = 241;
		brick = std::string( "2446" );

		if( Stan_PL-50<min ){
			check_PL = 11 - (int)((50 - Stan_PL + min)/10);
		}
		else if( Stan_PL+50>max ){
			check_PL = 11 - (int)((Stan_PL + 50 - max)/10);
		}
		else{
			check_PL =11;
		}
	}
	else if(Stan_PL <= 691){
		max = 691;
		min = 471;
		brick = std::string( "4769" );

		if( Stan_PL-50<min ){
			check_PL = 11 - (int)((50 - Stan_PL + min)/10);
		}
		else if( Stan_PL+50>max ){
			check_PL = 11 - (int)((Stan_PL + 50 - max)/10);
		}
		else{
			check_PL =11;
		}
	}
	else {
		std::cout << std::endl;
		std::cout << " The PL number of the track you entered is not correct. " << std::endl;
		std::cout << "The PL number you entered is " << Stan_PL << " " << std::endl;
		std::cout << std::endl;
		exit (1);
	}
	return std::make_tuple(max, min, check_PL, brick );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

double dif_ang(double ref_ax, double ref_ay, double ax, double ay){
	double dax;
	double day;
	double da;

	dax = tan(std::atan(ref_ax) - std::atan(ax));
	day = tan(std::atan(ref_ay) - std::atan(ay));
	da = sqrt(pow(dax,2) + pow(day,2));
	return da;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::pair< double, double> track_Extrapolation(double ax, double ay, double x, double y, double z, double after_z){
	double x_conversion;
	double y_conversion;

	x_conversion =  (after_z - z)*ax + x;
	y_conversion =  (after_z - z)*ay + y;

	return std::make_pair(x_conversion, y_conversion);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool cut_coordinate_cylinder(double stan_track_ax, double stan_track_ay, double stan_track_x, double stan_track_y, double stan_track_z, double stan_track_after_z, double judge_x, double judge_y, double cut_val){

	double Standard_x;
	double Standard_y;
	double distance;

	std::tie(Standard_x, Standard_y) = track_Extrapolation(stan_track_ax, stan_track_ay, stan_track_x, stan_track_y, stan_track_z, stan_track_after_z);
	distance = sqrt(pow((Standard_x - judge_x), 2) + pow((Standard_y - judge_y), 2));
	if(distance<cut_val){
		return true;
	}
	else{
		return false;
	}
}


// cylinder cut の bool
bool cylinder_cut_bool(double Ref_ax, double Ref_ay, double Ref_x, double Ref_y , double Ref_z, double after_z, double x, double y, double ax, double ay, double radius, double Ang_diff){
	if(cut_coordinate_cylinder(Ref_ax, Ref_ay, Ref_x, Ref_y, Ref_z, after_z, x, y, radius) && dif_ang(Ref_ax, Ref_ay, ax, ay)<Ang_diff){
		return true;
	}
	else{
		return false;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool cut_coordinate_corn(double stan_track_ax, double stan_track_ay, double stan_track_x, double stan_track_y, double stan_track_z, double stan_track_after_z, double judge_x, double judge_y, double cut_val,int diff_PL){
	double Standard_x;
	double Standard_y;
	double distance;

	std::tie(Standard_x, Standard_y) = track_Extrapolation(stan_track_ax, stan_track_ay, stan_track_x, stan_track_y, stan_track_z, stan_track_after_z);
	distance = sqrt(pow((Standard_x - judge_x), 2) + pow((Standard_y - judge_y), 2));
	if(distance<cut_val*diff_PL*0.5){
		return true;
	}
	else{
		return false;
	}

}

// corn cut の bool
bool corn_cut_bool(double Ref_ax, double Ref_ay, double Ref_x, double Ref_y , double Ref_z, double after_z, double x, double y, double ax, double ay, double radius, int diff_PL, double Ang_diff){
	if(cut_coordinate_corn(Ref_ax, Ref_ay, Ref_x, Ref_y, Ref_z, after_z, x, y, radius, diff_PL) && dif_ang(Ref_ax, Ref_ay, ax, ay)<Ang_diff){
		return true;
	}
	else{
		return false;
	}
}



// ip & md
class various_processing{

	double ip_unit_vec_ax;
	double ip_unit_vec_ay;
	double ip_unit_vec_az;
	double md_unit_vec_ax;
	double md_unit_vec_ay;
	double md_unit_vec_az;
	double Ref_md_unit_vec_ax;
	double Ref_md_unit_vec_ay;
	double Ref_md_unit_vec_az;
	double Ref_size_of_ang;
	double size_of_ang;
	double para_t; // 媒介変数
	double md_parameter_t;
	double md_parameter_s;

	int ph;
	int vph;

public:
	double Ref_x;
	double Ref_y;
	double Ref_z;
	double Ref_ax;
	double Ref_ay;
	double Ref_az;
	double ip_dx;
	double ip_dy;
	double ip_dz;
	double md_dx;
	double md_dy;
	double md_dz;
	double ip_x;
	double ip_y;
	double ip_z;
	double md_x[2];
	double md_y[2];
	double md_z[2];

	int track_ID;
	int PL;
	double ax;
	double ay;
	double az;
	double x;
	double y;
	double z;
	int phphv_1;
	int phphv_2;
	int line;

	double numb_of_ip;
	double numb_of_md;

	double judge_numb_of_md[2];

	char phvph_buf[48];

	void set_phphv(){
		ph  = ( phphv_1/10000 + phphv_2/10000 );
		vph = ( phphv_1%10000 + phphv_2%10000 );
	}

	void set_prm(std::string tmp){
		sscanf(tmp.data(), "%d %d %*d %lf %lf %lf %lf %d %*f %*f %*f %*f %*f %*d %*d %*d %*d %*d %*d %d" , &track_ID, &PL, &ax, &ay, &x, &y, &phphv_1, &phphv_2);
		set_phphv();
		line++;
	}

	void initialization(double z_file){
		line = -1;
		z = z_file;
		Ref_az = 1;
		az = 1;
	}

	void ip_Unit_vector(){
		size_of_ang = sqrt(pow(ax,2)+pow(ay,2)+pow(az,2));
		ip_unit_vec_ax = ax / size_of_ang;
		ip_unit_vec_ay = ay / size_of_ang;
		ip_unit_vec_az = az / size_of_ang;
	}
	void md_Unit_vector(){
		Ref_size_of_ang = sqrt(pow(Ref_ax,2)+pow(Ref_ay,2)+pow(Ref_az,2));
		size_of_ang = sqrt(pow(ax,2)+pow(ay,2)+pow(az,2));
		Ref_md_unit_vec_ax = Ref_ax / Ref_size_of_ang;
		Ref_md_unit_vec_ay = Ref_ay / Ref_size_of_ang;
		Ref_md_unit_vec_az = Ref_az / Ref_size_of_ang;
		md_unit_vec_ax = ax / size_of_ang;
		md_unit_vec_ay = ay / size_of_ang;
		md_unit_vec_az = az / size_of_ang;
	}

	double inner_product(double a1, double a2, double a3, double b1, double b2, double b3){
		return a1*b1 + a2*b2 + a3*b3;
	}

	void Set_ip_parameter(){
		para_t = (Ref_x - x)*ip_unit_vec_ax + (Ref_y - y)*ip_unit_vec_ay + (Ref_z - z)*ip_unit_vec_az;
	}
	void Set_md_parameter(){
		double l_AB = inner_product(Ref_md_unit_vec_ax, Ref_md_unit_vec_ay, Ref_md_unit_vec_az, x - Ref_x, y - Ref_y, z - Ref_z);
		double m_AB = inner_product(md_unit_vec_ax, md_unit_vec_ay, md_unit_vec_az, x - Ref_x, y - Ref_y, z - Ref_z);
		double l_m = inner_product(Ref_md_unit_vec_ax, Ref_md_unit_vec_ay, Ref_md_unit_vec_az, md_unit_vec_ax, md_unit_vec_ay, md_unit_vec_az);
		md_parameter_t = ( l_AB - l_m*m_AB )/( 1 - pow(l_m,2) );
		md_parameter_s = (l_AB*l_m - m_AB)/( 1 - pow(l_m,2) );
	}


	void Set_ip_point(){
		ip_x = x + ip_unit_vec_ax*para_t;
		ip_y = y + ip_unit_vec_ay*para_t;
		ip_z = z + ip_unit_vec_az*para_t;
	}
	void Set_md_point(){
		md_x[0] = Ref_x + md_parameter_t*Ref_md_unit_vec_ax;
		md_y[0] = Ref_y + md_parameter_t*Ref_md_unit_vec_ay;
		md_z[0] = Ref_z + md_parameter_t*Ref_md_unit_vec_az;
		md_x[1] = x + md_parameter_s*md_unit_vec_ax;
		md_y[1] = y + md_parameter_s*md_unit_vec_ay;
		md_z[1] = z + md_parameter_s*md_unit_vec_az;
	}

	// 対象の点と最近接点とのx,y,zの距離。
	void Set_ip_coordinate(){
		ip_dx = x + ip_unit_vec_ax*para_t - Ref_x;
		ip_dy = y + ip_unit_vec_ay*para_t - Ref_y;
		ip_dz = z + ip_unit_vec_az*para_t - Ref_z;
	}
	void Set_md_coordinate(){
		md_dx = md_x[1] - md_x[0];
		md_dy = md_y[1] - md_y[0];
		md_dz = md_z[1] - md_z[0];
	}

	void Calculation_ip_val(){
		numb_of_ip = sqrt( pow(ip_dx, 2) + pow(ip_dy, 2) +pow(ip_dz, 2) );
	}
	void Calculation_md_val(){
		numb_of_md = sqrt( pow(md_dx, 2) + pow(md_dy, 2) +pow(md_dz, 2) );
	}

	void Set_ip_all(){
		ip_Unit_vector();
		Set_ip_parameter();
		Set_ip_point();
		Set_ip_coordinate();
		Calculation_ip_val();
	}
	void Set_md_all(){
		md_Unit_vector();
		Set_md_parameter();
		Set_md_point();
		Set_md_coordinate();
		Calculation_md_val();
	}



	void output_mfile(std::ofstream& m_of){
		PL = PL*10 + 1;
		sprintf(phvph_buf, "%04d", vph);
		m_of << line << " " << "1" << " " << PL << " " << PL << std::endl;
		m_of << PL << " " << line << " " << track_ID << " " << ph << phvph_buf << " " << ax << " " << ay << " " << x << " " << y << " " << z << "   0  0  0  0  0.0  0.0" << std::endl;
	}

};



bool cut_circular_md( double md_x[], double md_y[], double md_z[], double stan_x, double stan_y, double stan_z, double tolerance){

	double comparison_coordinates_x = (md_x[0] + md_x[1])/2;
	double comparison_coordinates_y = (md_y[0] + md_y[1])/2;
	double comparison_coordinates_z = (md_z[0] + md_z[1])/2;

	double dx = comparison_coordinates_x - stan_x;
	double dy = comparison_coordinates_y - stan_y;
	double dz = comparison_coordinates_z - stan_z;

	double distance = sqrt(pow(dx,2)+pow(dy,2)+pow(dz,2));

	if(distance < tolerance){
		return true;
	}
	else{
		return false;
	}


}


bool check_cut_bool(double x, double y, double ax, double ay, double md_x[], double md_y[], double md_z[], double stan_ax, double stan_ay, double stan_x, double stan_y, double stan_z, double after_z, double radius, int diff_PL, double Ang_diff, double tolerance){


	if(corn_cut_bool(stan_ax, stan_ay, stan_x, stan_y, stan_z, after_z, x, y, ax, ay, radius, diff_PL, Ang_diff) || cylinder_cut_bool(stan_ax, stan_ay, stan_x, stan_y, stan_z, after_z, x, y, ax, ay, radius, Ang_diff) || cut_circular_md(md_x, md_y, md_z, stan_x, stan_y, stan_z, tolerance)){
		return true;
	}
	else{
		return false;
	}


}



bool check_val(std::string str){
	if(str == "non_argument"){
		return false;
	}
	else{
		return true;
	}
}



int main(int argc, char *argv[]){

	std::map<std::string, std::string> argment;
	argment = opt_process( argc, argv);

	class various_processing point;

	/////////////////////////////////////////
	std::string md_or_ip = argment["Judge_criteria"];
	int E = atoi(argment["ECC"].c_str());
	int A = atoi(argment["Area"].c_str());
	std::string brick;
	int event_numb = 0;
	if(check_val(argment["event_number"])){
		event_numb = atoi(argment["event_number"].c_str());
	}
	int Standard_PL = atoi(argment["PL"].c_str());
	int track_ID = 0;
	if(check_val(argment["track_ID"])){
		track_ID = atoi(argment["track_ID"].c_str());
	}
	int phphv = 0;
	if(check_val(argment["phvph"])){
		phphv = atoi(argment["phvph"].c_str());
	}
	point.Ref_ax = 0;
	if(check_val(argment["Ref_ax"])){
		point.Ref_ax = atof(argment["Ref_ax"].c_str());
	}
	point.Ref_ay = 0;
	if(check_val(argment["Ref_ay"])){
		point.Ref_ay = atof(argment["Ref_ay"].c_str());
	}
	point.Ref_x = atof(argment["Ref_x"].c_str());
	point.Ref_y = atof(argment["Ref_y"].c_str());
	point.Ref_z = atof(argment["Ref_z"].c_str());
	std::string Origin_mfile = argment["Original_mfile"];
	double radius = 50;
	if(check_val(argment["radius"])){
		radius = atof(argment["radius"].c_str());
	}
	double Ang_diff = 0.3;
	if(check_val(argment["Accept_angle_difference"])){
		Ang_diff = atof(argment["Accept_angle_difference"].c_str());
	}
	double tolerance = 400;
	if(check_val(argment["tolerance"])){
		tolerance = atof(argment["tolerance"].c_str());
	}
	std::string output_filename = argment["output_file_name"];
	std::string prm_file = argment["parameter_file_name"];
	/////////////////////////////////////////

	int judge_bool;

	std::ifstream fin;
	std::string str;
	//char infile_name[2048];
	std::string infile_name;
	std::ifstream fin_zid;
	std::map<int, double> z_coordinate;
	int z_PL;
	double z_val;

	int check_N;
	int check_PL[11];
	char PL_buf[48];
	int max_PL;
	int min_PL;

	std::string Ans_overwrite = "hoge";
	std::string Ans_diff_file = "hoge";
	std::string Ans_new_file = "hoge";
	char eve_buf[48];

	if(md_or_ip =="md"){
		judge_bool = 0;
	}
	else if(md_or_ip =="ip"){
		judge_bool = 1;
	}
	else {
		std::cout << "The argument is incorrect." << std::endl;
		std::cout << "The program does nothing." << std::endl;
		exit(1);
	}

	if(!check_val(output_filename)){
		if(event_numb!=0 && track_ID!=0){
			sprintf(eve_buf, "s00%02d_", event_numb);
			output_filename =  eve_buf + std::to_string(Standard_PL) + "_" + std::to_string(track_ID) + "_" + md_or_ip + "200_follow_up_down.all";
			// output_filename = "md_200_track_search_5pl	}
		}
		else if(event_numb!=0){
			output_filename = std::to_string(Standard_PL) + "_" + std::to_string(track_ID) + "_" + md_or_ip + "200_follow_up_down.all";
		}
		else if(track_ID!=0){
			output_filename =  eve_buf + std::to_string(Standard_PL) + "_" + md_or_ip + "200_follow_up_down.all";
		}
		else{
			output_filename =  std::to_string(Standard_PL) + "_" + md_or_ip + "200_follow_up_down.all";
		}
	}
	while(file_exists(output_filename)){
		std::cout << " " << output_filename << " already exists " << std::endl;
		Ans_overwrite = "hoge";
		Ans_diff_file = "hoge";
		Ans_new_file = "hoge";
		while (!Ans_judge(Ans_overwrite)){
			std::cout << " Do you want to overwrite \"" << output_filename << "\" ?   (y/n)";
			std::cin >> Ans_overwrite;
			if (Ans_judge(Ans_overwrite) && !Y_N_bool(Ans_overwrite)) {
				while(!Ans_judge(Ans_diff_file)){
					std::cout << " Do you want to set a different file name ?   (y/n)";
					std::cin >> Ans_diff_file;
					if(Ans_judge(Ans_diff_file) && Y_N_bool(Ans_diff_file)){
						while(!Ans_judge(Ans_new_file)){
							std::cout << " What is the new file name ?   :   ";
							std::cin >> output_filename;
							std::cout << std::endl;
							std::cout << "Is the new file name \"" << output_filename << "\" ?   (y/n)";
							std::cin >> Ans_new_file;
							if(Ans_judge(Ans_new_file) && Y_N_bool(Ans_new_file)){
								std::cout << std::endl;
								std::cout << " new file name is \"" << output_filename << "\" !" << std::endl;
								std::cout << std::endl;
							}
							else {
								Ans_new_file = "hoge";
							}
						}
					}
					else if(Ans_judge(Ans_diff_file)){
						std::cout << " Exit the program. " << std::endl;
						std::cout << " No process is going on. " << std::endl;
						exit(1);
					}
				}
			}
			else{
				std::cout << std::endl;
				std::cout << " Overwrite \"" << output_filename << "\" " << std::endl;
				std::cout << std::endl;
			}
		}
		if( Y_N_bool(Ans_overwrite) ){
			break;
		}
	}
	std::cout << std::endl;
	std::cout << " \"" << output_filename << "\" was created. " << std::endl;
	std::cout << std::endl;
	std::ofstream fout(output_filename);

	std::tie(max_PL, min_PL, check_N, brick) = brick_PL_relation_various(Standard_PL);

	if (check_val(Origin_mfile)) {
		origin_mfile_output(Origin_mfile, fout);
	}
	else{
		fout << "% Created by mkmf" << std::endl;
		fout << "% on 2007/12/25 23:28:09 +09:00 (JST)" << std::endl;
		fout << "       0       0   3   0      0.0   0.0000" << std::endl;
		fout << "  23" << std::endl;
		if(brick == "0123"){
			fout << " 11 21 31 41 51 61 71 81 91 101 111 121 131 141 151 161 171 181 191 201 211 221 231" << std::endl;
		}
		else if(brick == "2446"){
			fout << " 241 251 261 271 281 291 301 311 321 331 341 351 361 371 381 391 401 411 421 431 441 451 461" << std::endl;
		}
		else{
			fout << " 471 481 491 501 511 521 531 541 551 561 571 581 591 601 611 621 631 641 651 661 671 681 691" << std::endl;
		}
	}


	if (check_N < 11 && Standard_PL-check_N*10<min_PL) {
		for (int i = 0; i < check_N; ++i) {
			check_PL[i] = (Standard_PL/10 - 5 + i + 11 - check_N)*10 + 1;
		}
	}
	else {
		for (int i=0; i < check_N; ++i) {
			check_PL[i] = (Standard_PL - 50 +i*10);
		}
	}


	infile_name = "/data2/T60/run6_2/ECC" + std::to_string(E) + "/Area" + std::to_string(A) + "/0/" + brick + "/mfile/zid.dat";

	fin_zid.open(infile_name);
	if(!fin_zid){
		std::cerr << " no exist filepath : " << strerror(errno) << std::endl;
		std::cerr << " print error code : " << errno << std::endl;
		std::cerr << " can not open " << infile_name <<  std::endl;
		exit(1);
	}

	while(getline(fin_zid, str)){
		sscanf(str.data(), "%d %lf", &z_PL, &z_val);
		z_coordinate[z_PL] = z_val;
	}

	for(int i=0; i<check_N; i++){

		// base track calculation //
		sprintf(PL_buf, "%02d", int(check_PL[i]/10));
		infile_name = "/data2/T60/run6_2/ECC" + std::to_string(E) + "/Area" + std::to_string(A) + "/PL" + PL_buf + "/b0" + PL_buf + ".sel.abs.dmp";
		std::cout << infile_name << std::endl;
		fin.open(infile_name);
		if(!fin){
			std::cerr << " no exist filepath : " << strerror(errno) << std::endl;
			std::cerr << " print error code : " << errno << std::endl;
			std::cerr << " can not open " << infile_name <<  std::endl;
			exit(1);
		}

		point.initialization(z_coordinate[check_PL[i]]);
		while (getline(fin,str)) {
			point.set_prm(str);
			if(judge_bool==0){
				point.Set_md_all();
				if(point.numb_of_md<200){
					if(check_cut_bool(point.x, point.y, point.ax, point.ay, point.md_x, point.md_y, point.md_z, point.Ref_ax, point.Ref_ay, point.Ref_x, point.Ref_y, point.Ref_z, z_coordinate[check_PL[i]], radius, abs(check_PL[i] - Standard_PL), Ang_diff, tolerance)){
						point.output_mfile(fout);
					}
				}
			}
			if(judge_bool==1){
				point.Set_ip_all();
				if(point.numb_of_ip<200){
					point.output_mfile(fout);
				}
			}
		}
		fin.close();
	}

	fout.close();
	return 0;
}
