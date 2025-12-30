// #ifndef VIEW_QUESTION
// #define VIEW_QUESTION
// #include "../Models/Entities/QuestionEntity.hpp"

// static inline std::string repeat_utf8(const std::string &ch, size_t n)
// {
// 	std::string out;
// 	out.reserve(ch.size() * n);
// 	for (size_t i = 0; i < n; ++i) out += ch;
// 	return out;
// }

// /*
// ┏━ ■ Team 1 ━ U0000001 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓
// ┃				  ANSWER SPOT 1 QUESTION TO OCCUPY					 ┃
// ┣━ QUESTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
// ┃ What is the speed of light in a vacuum?                                                                   ┃
// ┃                                                                    ┃
// ┃                                                                    ┃
// ┃                                                                    ┃
// ┣━ ANSWERS ━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
// ┃ 1:                               ┃ 3:                              ┃
// ┃ 2:                               ┃ 4:                              ┃
// ┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
// ┃ Please answer in 30s                                               ┃
// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
// */

// int timeLeftSeconds = 30;

// string WrapText(const string& text, int width)
// {
// 	stringstream in(text);
// 	string word;
// 	string line;
// 	string out;

// 	while (in >> word) {
// 		if ((int)line.size() + (int)word.size() + 1 > width) {
// 			out += line;
// 			out += '\n';
// 			line = word + " ";
// 		} else {
// 			line += word + " ";
// 		}
// 	}

// 	if (!line.empty()) {
// 		// trim trailing space
// 		if (line.back() == ' ') line.pop_back();
// 		out += line;
// 	}

// 	return out;
// }

// string MakeQuestionBlock(const string& question)
// {
// 	// available width inside box
// 	const int inner = WINDOW_WIDTH - 4;
// 	string wrapped = WrapText(question, inner);
// 	stringstream ss;

// 	ss << "┣━ QUESTION ━" << repeat_utf8(u8"━", 54) << "┫\n";

// 	// print up to 4 lines for question area (as in comment)
// 	vector<string> lines;
// 	string line;
// 	stringstream wss(wrapped);
// 	while (getline(wss, line)) lines.push_back(line);

// 	for (int i = 0; i < 4; ++i) {
// 		string t = (i < (int)lines.size()) ? lines[i] : "";
// 		ss << "┃ " << left << setw(inner) << t << " ┃\n";
// 	}

// 	return ss.str();
// }

// string MakeAnswersBlock(const vector<string>& answers)
// {
// 	// two-column layout
// 	const int colInner = (WINDOW_WIDTH - 6) / 2; // each column
// 	stringstream ss;
// 	ss << "┣━ ANSWERS ━" << repeat_utf8(u8"━", 43) << "┫\n";

// 	// ensure exactly 4 answers (pad if needed)
// 	array<string,4> a;
// 	for (int i = 0; i < 4; ++i) a[i] = (i < (int)answers.size()) ? answers[i] : "";

// 	// print two rows (1 & 3 left/right)
// 	ss << "┃ " << left << setw(colInner) << (string("1: ") + a[0]) << " ┃ " << left << setw(colInner) << (string("3: ") + a[2]) << " ┃\n";
// 	ss << "┃ " << left << setw(colInner) << (string("2: ") + a[1]) << " ┃ " << left << setw(colInner) << (string("4: ") + a[3]) << " ┃\n";

// 	return ss.str();
// }

// void ShowQuestionView(QuestionEntity CurrentQuestion)
// {
// 	ClearScreen();

// 	// cout << GetGameTitle();

// 	cout << MakeQuestionBlock(CurrentQuestion.content);
// 	cout << MakeAnswersBlock(CurrentQuestion.answers);

// 	cout << "┣━ CONSOLES ━" << repeat_utf8(u8"━", 55) << "┫\n";
// 	string prompt = "Please answer in " + to_string(timeLeftSeconds) + "s";
// 	cout << "┃ " << BOLD << prompt << RESET << string(WINDOW_WIDTH - 4 - (int)prompt.size(), ' ') << " ┃\n";
// 	cout << "┗" << repeat_utf8(u8"━", WINDOW_WIDTH - 2) << "┛\n";
// }

// void ShowQuestionLog(string log)
// {
// 	// preserve interface but update console log
// 	// if there is a current question in global scope, we could reuse it; for now just print log
// 	ClearScreen();
// 	// cout << GetGameTitle();
// 	cout << "┣━ QUESTION ━" << repeat_utf8(u8"━", 55) << "┫\n";
// 	cout << "┃ " << RESET << string(WINDOW_WIDTH - 4, ' ') << " ┃\n";
// 	cout << "┣━ ANSWERS ━" << repeat_utf8(u8"━", 56) << "┫\n";
// 	cout << "┃ " << RESET << string(WINDOW_WIDTH - 4, ' ') << " ┃\n";
// 	cout << "┣━ CONSOLES ━" << repeat_utf8(u8"━", 56) << "┫\n";
// 	cout << "┃ " << BOLD << log << RESET << string(WINDOW_WIDTH - 4 - log.size(), ' ') << " ┃\n";
// 	cout << "┗" << repeat_utf8(u8"━", WINDOW_WIDTH - 2) << "┛\n";
// }





// #endif


#ifndef VIEW_QUESTION
#define VIEW_QUESTION
#include "../Models/Entities/InGameEntity.hpp"
#include <thread>
#include <atomic>
#include <chrono>

static inline std::string repeat_utf8(const std::string &ch, size_t n)
{
	std::string out;
	out.reserve(ch.size() * n);
	for (size_t i = 0; i < n; ++i) out += ch;
	return out;
}

/*
┏━ ■ Team 1 ━ U0000001 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 00:00:00 ━┓
┃                  ANSWER SPOT 1 QUESTION TO OCCUPY                  ┃
┣━ QUESTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ What is the speed of light in a vacuum?                            ┃
┃                                                                    ┃
┃                                                                    ┃
┃                                                                    ┃
┣━ ANSWERS ━━━━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ 1:                               ┃ 3:                              ┃
┃ 2:                               ┃ 4:                              ┃
┣━ OPTION ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ • 1 <answerID> : Answer question ┃ • 2 : Return map view 		     ┃
┣━ CONSOLES ━━━━━━━━━━━━━━━━━━━━━━━┻━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
┃ Please answer in 30s                                               ┃
┃                                                                    ┃
┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
*/

inline std::atomic<int> timeLeftSeconds{QuestionTimeOut};
inline std::atomic<bool> countdownRunning{false};
inline std::thread countdownThread;

void StopQuestionCountdown();
void StartQuestionCountdown(QuestionEntity CurrentQuestion, int QuestionTimeOut);

string WrapText(const string& text, int width)
{
	stringstream in(text);
	string word;
	string line;
	string out;

	while (in >> word) {
		if ((int)line.size() + (int)word.size() + 1 > width) {
			out += line;
			out += '\n';
			line = word + " ";
		} else {
			line += word + " ";
		}
	}

	if (!line.empty()) {
		if (line.back() == ' ') line.pop_back();
		out += line;
	}

	return out;
}

string MakeQuestionBlock(const string& question)
{
	const int inner = WINDOW_WIDTH - 4;
	string wrapped = WrapText(question, inner);
	stringstream ss;

	ss << "┣━ QUESTION ━" << repeat_utf8("━", 55) << "┫\n";

	vector<string> lines;
	string line;
	stringstream wss(wrapped);
	while (getline(wss, line)) lines.push_back(line);

	for (int i = 0; i < 4; ++i) {
		string t = (i < (int)lines.size()) ? lines[i] : "";
		ss << "┃ " << left << setw(inner) << t << " ┃\n";
	}

	return ss.str();
}

string MakeAnswersBlock(const vector<string>& answers)
{
	const int colInner = (WINDOW_WIDTH - 6) / 2;
	stringstream ss;
	
	ss << "┣━ ANSWERS ━" << repeat_utf8("━", 26) << "┳" << repeat_utf8("━", 30) << "┫\n";

	array<string,4> a;
	for (int i = 0; i < 4; ++i) a[i] = (i < (int)answers.size()) ? answers[i] : "";

	ss << "┃ " << left << setw(colInner) << (string("1: ") + a[0]) << " ┃ " << left << setw(colInner) << (string("3: ") + a[2]) << " ┃\n";
	ss << "┃ " << left << setw(colInner) << (string("2: ") + a[1]) << " ┃ " << left << setw(colInner) << (string("4: ") + a[3]) << " ┃\n";

	return ss.str();
}

void ShowQuestionView(QuestionEntity CurrentQuestion)
{
	ClearScreen();

	string titleText = "ANSWER QUESTION TO OCCUPY";
	int padding = (WINDOW_WIDTH - 4 - (int)titleText.size()) / 2;
	cout << "┃ " << string(padding, ' ') << titleText << string(WINDOW_WIDTH - 4 - padding - (int)titleText.size(), ' ') << " ┃\n";

	cout << MakeQuestionBlock(CurrentQuestion.content);
	cout << MakeAnswersBlock(CurrentQuestion.answers);


	const int colInner = (WINDOW_WIDTH - 6) / 2;
	cout << "┣━ OPTION ━" << repeat_utf8("━", 58) << "┫\n";
	cout << "┃ " << left << setw(colInner) << "• 1 <answerID> : Answer question" << " ┃ " << left << setw(colInner) << "• 2 : Return map view" << " ┃\n";

	
	cout << "┣━ CONSOLES ━" << repeat_utf8("━", 26) << "┻" << repeat_utf8("━", 29) << "┫\n";

	string prompt = "Please answer in " + to_string((int)timeLeftSeconds.load()) + "s";
	cout << "┃ " << BOLD << prompt << RESET << string(WINDOW_WIDTH - 4 - (int)prompt.size(), ' ') << " ┃\n";
	string logLine = ""; // empty log line in regular view
	cout << "┃ " << RESET << logLine << string(WINDOW_WIDTH - 4 - (int)logLine.size(), ' ') << " ┃\n";
	cout << "┗" << repeat_utf8("━", WINDOW_WIDTH - 2) << "┛\n";
}

void StopQuestionCountdown()
{
	countdownRunning = false;
	if (countdownThread.joinable()) countdownThread.join();
}

void StartQuestionCountdown(QuestionEntity CurrentQuestion, int seconds)
{
	StopQuestionCountdown();
	timeLeftSeconds = seconds;
	countdownRunning = true;
	countdownThread = std::thread([CurrentQuestion]() mutable {
		while (countdownRunning && timeLeftSeconds.load() > 0) {
			ShowQuestionView(CurrentQuestion);
			std::this_thread::sleep_for(std::chrono::seconds(1));
			timeLeftSeconds.fetch_sub(1);
		}
		countdownRunning = false;
	});
}

void ShowQuestionLog(QuestionEntity CurrentQuestion, string log)
{
	ClearScreen();
	
	cout << GetGameTitle();
	
	string titleText = "ANSWER QUESTION TO OCCUPY";
	int padding = (WINDOW_WIDTH - 4 - (int)titleText.size()) / 2;
	cout << "┃ " << string(padding, ' ') << titleText << string(WINDOW_WIDTH - 4 - padding - (int)titleText.size(), ' ') << " ┃\n";

	cout << MakeQuestionBlock(CurrentQuestion.content);
	cout << MakeAnswersBlock(CurrentQuestion.answers);


	const int colInner = (WINDOW_WIDTH - 6) / 2;
	cout << "┣━ OPTION ━" << repeat_utf8("━", 58) << "┫\n";
	cout << "┃ " << left << setw(colInner) << "• 1 <answerID> : Answer question" << " ┃ " << left << setw(colInner) << "• 2 : Return map view" << " ┃\n";


	cout << "┣━ CONSOLES ━" << repeat_utf8("━", 26) << "┻" << repeat_utf8("━", 29) << "┫\n";
	string prompt = "Please answer in " + to_string((int)timeLeftSeconds.load()) + "s";
	cout << "┃ " << BOLD << prompt << RESET << string(WINDOW_WIDTH - 4 - (int)prompt.size(), ' ') << " ┃\n";
	cout << "┃ " << BOLD << log << RESET << string(WINDOW_WIDTH - 4 - (int)log.size(), ' ') << " ┃\n";
	cout << "┗" << repeat_utf8("━", WINDOW_WIDTH - 2) << "┛\n";
}




#endif